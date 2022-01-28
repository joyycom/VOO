#ifndef ParticleColorComponent_H
#define ParticleColorComponent_H

#include "BaseComponent.h"

#include <QColor>
#include <QUrl>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

class ParticleColorComponent : public BaseComponent {
    Q_OBJECT
public:
    ParticleColorComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QVariantList getPartileColors(){
        return m_colors;
    }

    Q_INVOKABLE QString getLabel(){

        return translate(m_label);
    }

    Q_INVOKABLE void setPartileColors(QVariantList data);

    void onOfParamsChanged(QString key, SkyVariant v) override;
signals:

    void partileColorDataChanged();


private:
    void loadPartileColorData();

    QString m_label = "";
    QString m_defCfgPath = "";
    ParamSettingInfo m_defParam;
    QVariantList m_colors;
};

#endif // ParticleColorComponent_H
