#include "OrangeFilterUtils.h"

#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QFileInfo>
#include "SharedPref.h"
#include "project/SEProject.h"
#include "src/base/utils/AppConstance.h"
#include "src/base/utils/AppPath.h"
#include <QDebug>

void OrangeFilterUtils::openEffect(QString effectPath) {
#ifdef Q_OS_WIN
    auto ofPath = SharedPref::instance()->readString("of_exe_path");
    if (!QFileInfo(ofPath).exists()) {
        qWarning() << "OF Path is NOT Exists!" << ofPath;
        return;
    }

    QString resFile = SEProject::current()->dom()->timelineController()->timelineModel()->firstVideoTrackResource();

    QString s;
    s = QCoreApplication::applicationDirPath() +
        "/orangefilter_designer/of_designer.exe "
        "-NormalEffectPath \"" +
        effectPath +
        "\" "
        "-NormalEffectInput0 \"" +
        resFile + "\"";
    QProcess *proc = new QProcess();
    proc->start(s);
    proc->waitForStarted();
    proc->waitForFinished();
    // QString out = QString::fromLocal8Bit(proc->readAllStandardOutput());
#endif
}

static QMap<QString, QString> ttfMap = {{"zh_CN", "zh-hans.ttf"},
                                        {"zh_HK", "zh-hant.ttf"},
                                        {"zh_TW", "zh-hant.ttf"},
                                        {"th_TH", "th.ttf"},
                                        {"ja_JP", "ja.ttf"},
                                        {"ko_KR", "ko.ttf"},
                                        {"*", "en.ttf"}};

static QMap<QLocale::Language, QString> languageTtfMap = {{QLocale::Chinese, "zh-hans.ttf"},
                                        {QLocale::Thai, "th.ttf"},
                                        {QLocale::Japanese, "ja.ttf"},
                                        {QLocale::Korean, "ko.ttf"}};

QString OrangeFilterUtils::getLocalTTF() {
    QStringList fontTTF = {};
    auto locate = AppConstance::locate();
    QString locateName = locate.name();
    if(ttfMap.contains(locateName)){
        fontTTF.append(ttfMap[locateName]);
    }else{
        if(languageTtfMap.contains(locate.language())){
            fontTTF.append(languageTtfMap[locate.language()]);
        };
    }
    if(fontTTF.isEmpty()){
        qDebug()<<"Not Found ttf! uiLanguages:"<<locate.uiLanguages()<<" name:"<<locateName<<" language:"<<locate.language();
    }

    fontTTF.append("en.ttf");
    return fontTTF.join(",");
}
