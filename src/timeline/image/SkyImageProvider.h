#pragma once

#include "ImageCoverLoader.h"
#include "SkyImageLoader.h"
#include <QQuickAsyncImageProvider>

class SkyImageProvider : public QQuickAsyncImageProvider {
public:
    SkyImageProvider();

    virtual QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

    virtual Flags flags() const override {
        return ForceAsynchronousImageLoading;
    }

    void clear();
    
private:
    struct DecodeInfo {
        SkyResource resource;
        QString thumbDirPath;
    };
    QQuickImageResponse *doRequestImage(const QString& path, double time, const QSize &size);
    DecodeInfo findDecodeInfo(const QString& path);
    
private:
    QMap<QString, DecodeInfo> m_videoDecodeInfos;
    ImageCoverLoader m_imageLoader;
    QMutex mLock;
};

