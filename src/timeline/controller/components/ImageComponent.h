#ifndef ImageComponent_H
#define ImageComponent_H

#include "BaseComponent.h"

#include <QUrl>

class ImageComponent : public BaseComponent {
    Q_OBJECT
public:
    ImageComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE void updateImage(QUrl path);
    Q_INVOKABLE QString imagePath() const;
    Q_INVOKABLE QString labelText() const;

    void onOfParamsChanged(QString key, SkyVariant v) override;
signals:
    void imagePathUpdate(QString path);


private:
    void updateCurImagePath();

    void loadEffectParamInfo(QJsonValue& setting);



    bool m_isUpdateClipResource = false;
    QString m_imagePath = "";
    QString m_param = "";
    QString m_labelImage = "";
};

#endif // ImageComponent_H
