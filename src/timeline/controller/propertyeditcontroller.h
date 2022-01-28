#ifndef PROPERTYEDITCONTROLLER_H
#define PROPERTYEDITCONTROLLER_H

#include <QList>
#include <QList>
#include <QObject>
#include <timeline/models/effectgridpropertymodel.h>
#include <timeline/models/baseclip.h>
#include <timeline/models/effectclip.h>
#include <timeline/controller/components/BaseComponent.h>
#include <timeline/controller/components/ComponentBean.h>
#include <timeline/controller/components/LayoutComponent.h>
#include <timeline/controller/components/SimpleTransformComponent.h>
#include <timeline/controller/components/TransformComponent.h>

class PropertyEditController : public QObject {
    Q_OBJECT
public:
    //    Q_PROPERTY(QStringList componentQmls READ componentQmls NOTIFY componentQmlsChanged)
    Q_PROPERTY(int videoClipVolume READ videoClipVolume WRITE setVideoClipVolume NOTIFY videoClipVolumeChanged)
    explicit PropertyEditController(QObject *parent = nullptr);
    ~PropertyEditController();

    Q_INVOKABLE void onLoadCompleted();

    Q_INVOKABLE int videoClipVolume();

    Q_INVOKABLE void setVideoClipVolume(int volume);

    Q_INVOKABLE QString effectEditWidgetJSON();

    Q_INVOKABLE EffectGridPropertyModel *convertToGridModel(QJsonArray jsonArray);

    Q_INVOKABLE QString rootQml();

    Q_INVOKABLE void attachComponent(int index, QObject *component);

    Q_INVOKABLE void attachTransformComponent(TransformComponent *component);

    Q_INVOKABLE void attachSimpleTransformComponent(SimpleTransformComponent *component);

    Q_INVOKABLE void selectClipFromPreview(int x, int y, int previewWidth, int previewHeight);

    Q_INVOKABLE TransformComponent *transformComponent() {
        return m_transformComponent;
    }

    BaseClip *curSelectedClip();

    void receiveOfMessage(BaseClip *clip, QString msg);

    bool isPosInRotationRect(QPoint point, QRect rect, int angle);

    void updateUI();

    void updateCurFocusEffect(SkyEffectWrapper* wrapper);

    void showToast(QString msg, int during = 3000);

signals:

    void videoClipVolumeChanged(int volume);

    void componentQmlsChanged(QStringList qmls);

    void editPanelDataChanged();

    void toastContentChanged(QString msg, int during);

private slots:
    void onSelectedClipChanged(bool isSelected, BaseClip *clip);

    void onTimelineRemoveClipData(BaseClip *clip);

    void onTimelineClipDataChanged(BaseClip *clip);

private:
    void unBindComponents();

    SimpleTransformComponent *m_simpleTransformComponent = nullptr;

    TransformComponent *m_transformComponent = nullptr;
    ComponentBean* m_rootComponent = nullptr;
    BaseClip *m_curSelectedClip = nullptr;
    EffectGridPropertyModel *model = new EffectGridPropertyModel();
};

#endif // PROPERTYEDITCONTROLLER_H
