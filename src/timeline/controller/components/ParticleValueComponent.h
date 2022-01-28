#ifndef ParticleValueComponent_H
#define ParticleValueComponent_H

#include "BaseComponent.h"

#include <QColor>
#include <QUrl>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

class ParticleValueComponent : public BaseComponent {
    Q_OBJECT
public:
    ParticleValueComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QVariantList getPartileDatas(){
        return m_datas;
    }

    Q_INVOKABLE QString getLabel(){
        return translate(m_label);
    }

    Q_INVOKABLE void setPartileDatas(QVariantList data);

    void onOfParamsChanged(QString key, SkyVariant v) override;
signals:

    void partileDataChanged();


private:

    void loadPartileDatas();

    QString m_label = "";
    ParamSettingInfo m_defParam;
    QVariantList m_datas;
};

#endif // ParticleValueComponent_H
