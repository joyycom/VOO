#ifndef EffectGroupEditorComponent_H
#define EffectGroupEditorComponent_H

#include "BaseComponent.h"

class EffectGroupEditorComponent : public BaseComponent {
    Q_OBJECT

public:
    EffectGroupEditorComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString dropdownText() const{
        return m_paramText;
    }

    Q_INVOKABLE QStringList dataList() const{
        return m_dataList;
    }

    Q_INVOKABLE void setCurGroup(QString group);

    Q_INVOKABLE QString getCurGroup(){
        return m_curGroup;
    }

signals:
    void curGroupChanged(QString group);

private:
    QString m_curGroup = "";
    QString m_paramText = "";
    QString m_paramKey = "";
    QStringList m_dataList;
};

#endif // EffectGroupEditorComponent_H
