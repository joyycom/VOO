#ifndef UploadUtils_H
#define UploadUtils_H

#include <functional>
#include <QString>

class UploadUtils {
public:

    static void upload(QString filePath,
                       std::function<void(qint64, qint64)> progressHandler,
                       std::function<void(const bool, const QString &)> complateHandler, int retryCount = 0);
};

#endif // UploadUtils_H
