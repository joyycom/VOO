#include <QDir>
#include <QGuiApplication>
#include <QIcon>
#include <QOpenGLContext>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <QTextCodec>
#include <QTranslator>
#include <QStandardPaths>

#include "BuildConfig.h"
#include "base/utils/AppConstance.h"
#include "base/utils/AppPath.h"
#include "base/utils/CrashHelper.h"
#include "base/utils/SharedPref.h"
#include "base/utils/fileutils.h"
#include "base/utils/qmlutilities.h"
#include "base/utils/qt-detect-user-country.h"
#include "base/utils/threadpools.h"
#include "log.h"
#include "timeline/image/SkyImageProvider.h"
#include "timeline/qrcode/QRCodeImageProvider.h"
#include "ui/preview/PreviewController.h"
#include "user/Login.h"
#include "extension/VooExtension.h"
#include "extension/VooCommand.h"
#include "src/timeline/venus/VenusManager.h"

// If the application is release mode,the conlose need be hide
#if (NDEBUG && SKYEDITOR_WIN32)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

static void initGLEnvironment() {
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);

    // Request OpenGL 2.1 core or OpenGL ES 3.0.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        qDebug("Requesting 2.1 core context");
        fmt.setVersion(2, 1);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    } else {
        qDebug("Requesting 3.0 context");
        fmt.setVersion(3, 0);
    }

    QSurfaceFormat::setDefaultFormat(fmt);
}

static void onLogEvent(int level, const char *msg, int msgsize) {
    VooExtension::printLog(level, msg);
    switch (level) {
        case SkyLogLevel_Debug:
            qDebug() << msg;
            break;
        case SkyLogLevel_Info:
            qInfo() << msg;
            break;
        case SkyLogLevel_Warn:
            qWarning() << msg;
            break;
        case SkyLogLevel_Error:
            qCritical() << msg;
            break;
        default:
            qDebug() << msg;
            break;
    }
}

void initI18n(QGuiApplication &app) {
    QLocale locale = QLocale(QLocale::Language::AnyLanguage, LocalizationHelpers::detectUserCountry());
    QTranslator *translator = new QTranslator;
    QString localeName = locale.name();
    QString language2use = "editor_" + localeName.replace("_", "-") + ".qm";
    QString qmPath = QCoreApplication::applicationDirPath() + "/translations/" + language2use;
    qDebug() << "initI18n Locale: " << locale << " qmPath:" << qmPath;
    if (QFile::exists(qmPath) && translator->load(qmPath)) {
        app.installTranslator(translator);
    } else {
        qDebug() << "Load translator failed: " << qmPath;
    }
}

int main(int argc, char *argv[]) {
    //命令行处理
    if (VooCommand::check(argc, argv)) {
        return 0;
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    initI18n(app); //导入.qm翻译文件使用

#ifndef Q_OS_WINDOWS
    if (REMOTE_BUILD) {
        CrashHelper helper;
    }
#else
    CrashHelper helper;
#endif

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QFont font("Microsoft YaHei");
    font.setPixelSize(12);
    app.setFont(font);

    SkyApi::initSDK();
    SkyApi::setMaxNumberOfHardwareDecoder(3);
    SkyApi::registerLogCallback(onLogEvent);
    
    QDir venusBaseDir(QCoreApplication::applicationDirPath().append("/venus_models/"));
    VenusManager::sharedInstance().setVenusBaseDir(venusBaseDir.absolutePath().toUtf8().data());
    
    qInstallMessageHandler(SEMessageOutput);
    ThreadPools::instance()->initMainThread();
    //不要设置名字，设了会导致缓存目录乱掉
    // app.setOrganizationName("SkyEditor");
    app.setWindowIcon(QIcon(":/icon/assets/skyeditor.ico"));

    //初始化登录
    Login::instance()->init();
    Login::instance()->autoLogin(AppConstance::uid());

    // VOO扩展
    VooExtension::init();

    qWarning() << "=====SkyMedia Version: " << QString::fromUtf8(SkyApi::getSDKVersion())
               << " App Version:" << AppConstance::appVerWithBuildNum() << " argc:" << argc;

    QmlUtilities::registerCommonTypes();
    initGLEnvironment();
    const QUrl url(QStringLiteral("qrc:/ui/GuidePage.qml"));
    QQmlApplicationEngine *engine = QmlUtilities::sharedEngine();

    engine->addImageProvider(QLatin1String("skyimage"), new SkyImageProvider());
    engine->addImageProvider(QLatin1String("qrcode"), new QRCodeImageProvider());
    //    engine.rootContext()->setContextProperty("resourceMediaPlayer", new SkyMediaPlayer());

    engine->load(url);
    return app.exec();
}
