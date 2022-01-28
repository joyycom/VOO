#ifndef EffectInputListComponent_H
#define EffectInputListComponent_H

#include "BaseComponent.h"

#include <QUrl>

class EffectInputListComponent : public BaseComponent {
    Q_OBJECT

public:
    EffectInputListComponent(QObject *parent = nullptr);

    void onBindQml(QJsonObject &params) override;

    void onUnBind() override;

    Q_INVOKABLE QString dropdownText() const{
        return m_paramText;
    }

    Q_INVOKABLE void setCustomResPath(QUrl filePath);

    Q_INVOKABLE void setInputTypeIndex(int inputType);

    Q_INVOKABLE int getInputType(){
        return m_inputTypeIndex;
    }

    Q_INVOKABLE QString getCustomResPath(){
        return m_resPath;
    }

    Q_INVOKABLE QStringList inputTypes(){
        return m_inputTypeList;
    }

signals:

    void inputTypeChanged(int inputType);

    void customResPathChanged(QString filePath);

private:
    int INPUT_TYPE_BACKGROUND_INDEX = 0;
    int INPUT_TYPE_USER_INDEX = 1;
    int INPUT_TYPE_GAP_INDEX = 2;
    int INPUT_TYPE_PATH_INDEX = 3;

    QStringList m_inputTypeList;
    int m_inputTypeIndex = 0;
    int m_inputIndex = 0;
    QString m_paramText = "";
    QString m_resPath = "";
};

#endif // EffectInputListComponent_H
