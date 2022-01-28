#ifndef LOG_H
#define LOG_H

#include "qlogging.h"
#include <QMessageLogContext>
#include <QString>

void SEMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOG_H
