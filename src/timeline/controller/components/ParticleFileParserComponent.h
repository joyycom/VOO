#ifndef ParticleFileParserComponent_H
#define ParticleFileParserComponent_H

#include "BaseComponent.h"

#include <QColor>
#include <QUrl>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

class ParticleFileParserComponent : public BaseComponent {
    Q_OBJECT
public:
    ParticleFileParserComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE void setConfigFile(QString path);
    Q_INVOKABLE QString labelText(){
        return translate( m_label);
    }
signals:


private:
    QMap<QString, QVariant> loadParitclePlist(const QString& plistFile,const QString& effectConfigPath);


    QString m_label = "";
};

#endif // ParticleFileParserComponent_H
