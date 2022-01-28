#pragma once

#include <QQuickImageResponse>

class SkyImageResponse : public QQuickImageResponse {
public:
    SkyImageResponse();

    ~SkyImageResponse();

    virtual QQuickTextureFactory *textureFactory() const override;

    void onSuccess(QImage &image);

    bool isCancel() const {
        return isCanceled;
    }
    
public Q_SLOTS:
    void cancel() override;

private:
    QQuickTextureFactory *factory = nullptr;
    bool isCanceled = false;
};

