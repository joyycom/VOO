#ifndef ColorSelectorComponent_H
#define ColorSelectorComponent_H

#include "BaseComponent.h"

#include <QColor>
#include <QString>

class ColorSelectorComponent : public BaseComponent {
    Q_OBJECT
public:
    ColorSelectorComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString labelText();
    Q_INVOKABLE QColor selectColor();
    Q_INVOKABLE void updateColorSlient(QColor color);
    Q_INVOKABLE void updateColorByUser(QColor preColor, QColor color);

    void onOfParamsChanged(QString key, SkyVariant v) override;
signals:
    void colorUpdated(QColor color);

private:
    QString m_label;
    QColor m_color;
    QList<ParamSettingInfo> m_sliderParams;
};

#endif // ColorSelectorComponent_H
