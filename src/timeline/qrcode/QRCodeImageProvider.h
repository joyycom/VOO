#ifndef QRCodeImageProvider_H
#define QRCodeImageProvider_H

#include <QQuickAsyncImageProvider>

class QRCodeImageProvider : public QQuickAsyncImageProvider {
public:
    QRCodeImageProvider();

    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

    Flags flags() const override {
        return ForceAsynchronousImageLoading;
    }

    void clear();
};
#endif // QRCodeImageProvider_H
