#ifndef COMPONENT_BEAN_H
#define COMPONENT_BEAN_H

#include <QObject>
#include <QJsonObject>
#include <QMap>

#include <timeline/models/baseclip.h>

const QString PARAM_TYPE_FLOAT = "float";
const QString PARAM_TYPE_INT = "int";
const QString PARAM_TYPE_STRING = "string";
const QString PARAM_TYPE_BOOL = "bool";
const QString PARAM_TYPE_COLOR_RGBA = "float,float,float,float";
const QString PARAM_TYPE_COLOR_RGB = "float,float,float";


static QMap<QString, QString> m_componentDict = {{"Slider", "SliderProperty.qml"},
//                                                 {"Sticker", "AnimationListProperty.qml"},
                                                 {"Title", "LabelProperty.qml"},
                                                 {"Name", "NameProperty.qml"},
                                                 {"Word", "WordProperty.qml"},
                                                 {"Volume", "VolumeProperty.qml"},
                                                 {"Image", "ImageProperty.qml"},
                                                 {"Transition", "TransitionProperty.qml"},
                                                 {"CurvedSpeed", "CurvedSpeedProperty.qml"},
                                                 {"SimpleSpeed", "SimpleSpeedProperty.qml"},
                                                 {"VideoPicture", "VideoPictureSectionView.qml"},
                                                 {"VideoAdjustment", "VideoAdjustmentSectionView.qml"},
                                                 {"AnimationList", "AnimationListProperty.qml"},
                                                 {"EditTransform", "EditTransformProperty.qml"},
                                                 {"InputConfig", "EditInputProperty.qml"},
                                                 {"ColorSelector", "ColorSelectorProperty.qml"},
                                                 {"ColumnLayout", "PropertyColumnLayout.qml"},
                                                 {"StackLayout", "PropertyStackLayout.qml"},
                                                 {"SingleLayout", "PropertySingleLayout.qml"},
                                                 {"SubEffectList", "SubEffectListProperty.qml"},
                                                 {"ColumnLayoutNoScroll", "PropertyColumnLayoutNoScroll.qml"},
                                                 {"Text","MutilInputTextProperty.qml"},
                                                 {"TTS","TTSProperty.qml"},
                                                 {"DropdownList","DropDownListProperty.qml"},
                                                 {"Switch","SwitchProperty.qml"},
                                                 {"VolumeFade","VolumeFadeProperty.qml"},
                                                 {"EffectInputList","EffectInputListProperty.qml"},
                                                 {"EffectGroupEditor","EditGroupEditorProperty.qml"},
                                                 {"ParticleValue","ParticleValueProperty.qml"},
                                                 {"ParticleColor","ParticleColorProperty.qml"},
                                                 {"SaveOfEffect","SaveOfEffectProperty.qml"},
                                                 {"ParticleConfigParser","ParticleFileConfigProperty.qml"},
                                                 {"Empty", "EmptyProperty.qml"}};

static QMap<QString, int> m_componentEstimateHeight = {{"Slider", 40},
//                                                 {"Sticker", "AnimationListProperty.qml"},
                                                 {"Title",30},
                                                 {"Name", 30},
                                                 {"Volume", 40},
                                                 {"Image", 50},
                                                 {"Transition", 40},
                                                 {"SimpleSpeed", 40},
                                                       {"VolumeFade",40},
                                                 {"EditTransform", 120},
                                                 {"InputConfig", 120},
                                                 {"Text", 120},
                                                       {"Switch", 40},
                                                       {"DropdownList", 40},
                                                       {"EffectInputList",40},
                                                       {"EffectGroupEditor",40},
                                                       {"ParticleColor",60},
                                                       {"ParticleValue",120},
                                                       {"ParticleConfigParser",40},
                                                 {"SaveOfEffect",30},
                                                 {"ColorSelector", 80}};

class ParamSettingInfo {
public:
    ParamSettingInfo();
    ParamSettingInfo(QJsonObject &param);
    ParamSettingInfo(QJsonObject &param, ParamSettingInfo &info);

    QString paramName = "";
    int filterIndex = 0;
    QString defValue = "";
    double maxValue = 1;
    double minValue = 0;
    QString valueType = "";
    QVariant m_defValue;
    QString m_paramKey;
    int decimals = 0;
    QString m_prefix = "";
    QString m_suffix = "";

    QVariant defVal();

    QString paramKey();

    QString paramNameKey(){
        return paramName;
    }

    bool isValid(){
        return !paramName.isEmpty();
    }

    double stepSize();

    QString displayText(double val);

    QString displayTextPrefix();

    QString displayTextSuffix();

    double textToValue(QString text);

    double transformTargetParamValue(double curVal, ParamSettingInfo &target);
};

class ComponentBean {
public:
    ComponentBean(QString type, QString paramTitle, QJsonObject &param);
    ~ComponentBean();

    static ComponentBean* create(QString type, QString title, QJsonObject &param);

    QString qml = "";
    QString uiType = "";
    QString title = "";
    QJsonObject paramObj;
    BaseComponent *component = nullptr;
    QJsonObject m_translations;


    void bindQml(BaseClip *clip,SkyEffectWrapper* wrapper);

    void unBindQml();

    virtual void onUnbindQml(){

    }

    virtual void onClipParamsChanged();

    void setTransitions(QJsonObject& transitions);

    static BaseComponent* createComponent(QString type);

    virtual bool isGroupLayout(){
        return false;
    }


    virtual int estimateHeight();

    virtual QString toString();
};

static QString s_groupLayoutQmls[5] = {"PropertyColumnLayout.qml",
                             "PropertyColumnLayout.qml",
                             "PropertyStackLayout.qml",
                             "PropertySingleLayout.qml",
                             "PropertyColumnLayoutNoScroll.qml"};

static QString s_groupLayoutType[5] = {"ColumnLayout",
                             "ColumnLayout",
                             "StackLayout",
                             "SingleLayout",
                             "ColumnLayoutNoScroll"};

class GroupComponentBeans : public ComponentBean {

public:
    enum GroupLayout {
        DefaultLayout = 0,
        ColumnLayout = 1, //默认行布局 从上至下 依次排列
        StackLayout,      //堆栈布局 横向 多tab
        SingleLayout,      //单个包装
        ColumnLayoutNoScroll,
    };



    GroupComponentBeans(GroupLayout layout, QString type, QString paramTitle, QJsonObject &param);

    static GroupComponentBeans* create(QString id, QString title, QString layout);

    ~GroupComponentBeans();

    void onUnbindQml() override;

    void appendComponent(ComponentBean *component);

    ComponentBean *get(int index);

    bool isGroupLayout() override{
        return true;
    }

    void changeGroupUIType();

    void onClipParamsChanged() override;

    int estimateHeight() override;

    int count();

    QString toString() override;

private:
    QString m_id = "";
    QString m_title = "";
    GroupLayout m_layout = DefaultLayout;
    QList<ComponentBean *> m_components;
};

#endif // COMPONENT_BEAN_H
