#include "UnitConverUtils.h"
#include <QStringList>

QString UnitConverUtils::formatSize(qint64 size) {
    QStringList units = {"B", "KB", "MB", "GB", "TB", "PB"};
    int i;
    double outputSize = size;
    for (i = 0; i < units.size() - 1; i++) {
        if (outputSize < 1024)
            break;
        outputSize = outputSize / 1024;
    }
    return QString("%0%1").arg(outputSize, 0, 'f', 0).arg(units[i]);
}
