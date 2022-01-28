#ifndef ZipUtils_H
#define ZipUtils_H

#include <QString>

class ZipUtils {
public:
    static bool decompressFromZip(QString zipFile, QString outputDir);
    static QString compressFolder(QString dir, QString zipFilePath);

    static QString compressFile(QString filePath, QString zipFilePath);

private:
    static bool decompressFromZipBySystem(QString zipFile, QString outputDir);
    static QString compressFolderBySystem(QString dir, QString zipFilePath);
};
#endif // ZipUtils_H
