#include "AppPath.h"
#include "CrashHelper.h"
#include "sentry/sentry.h"
#include <QDebug>
#include <QCoreApplication>
#include "AppConstance.h"
#include "BuildConfig.h"

CrashHelper::CrashHelper(QObject *parent) : QObject(parent) {
    initCrash();
    qInfo() << "CrashHelper init";
}

CrashHelper::~CrashHelper() {

    sentry_shutdown();
    qInfo() << "CrashHelper destroy";
}

void sentryLoggerFunction(sentry_level_t level, const char *message, va_list args, void *userdata) {
    QString msg = QString::vasprintf(message, args);
    qDebug() << "sentry:" << level << " Message:" << msg;
}

void CrashHelper::initCrash() {

    sentry_options_t *options = sentry_options_new();
    sentry_options_set_dsn(options, "");
#ifdef Q_OS_WINDOWS
    QString crashpad = QCoreApplication::applicationDirPath().append("/crashpad_handler.exe");
#else
    QString crashpad = QCoreApplication::applicationDirPath().append("/../..").append("/crashpad_handler");
#endif
    qDebug() << "crashpad Handler Path:" << crashpad;

    auto crashDBPath = AppPath::crashPath();

    sentry_set_level(SENTRY_LEVEL_DEBUG);
    sentry_options_set_debug(options, !REMOTE_BUILD);
    sentry_options_set_database_path(options, crashDBPath.toStdString().c_str());
    sentry_options_set_handler_path(options, crashpad.toStdString().c_str());
    sentry_options_set_system_crash_reporter_enabled(options, false);
    sentry_options_set_release(options, AppConstance::appVerWithBuildNum().toUtf8().data());
    sentry_options_set_logger(options, &sentryLoggerFunction, nullptr);

    sentry_init(options);

    //    sentry_capture_event(sentry_value_new_message_event(
    //                             /*   level */ SENTRY_LEVEL_INFO,
    //                             /*  logger */ "11111",
    //                             /* message */ "windows!"
    //                             ));
}
