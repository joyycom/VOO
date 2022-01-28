#ifndef STORERESOURCEPREVIEWCONTROLLER_H
#define STORERESOURCEPREVIEWCONTROLLER_H

#include <QObject>

#include <timeline/models/baseclip.h>
#include <timeline/models/timelinemodel.h>

class StoreResourcePreviewController : public QObject {
    Q_OBJECT
public:
    explicit StoreResourcePreviewController(QObject *parent = nullptr);

    Q_INVOKABLE void attachModel();

    void startPreviewResource(SkyResourceBean &bean);

    void stopPreviewResource(SkyResourceBean &bean);

    void repreviewResource();

signals:
    //    void filterIntensityChanged(int intensity);
private slots:

private:
    void startPreviewEffect(SkyResourceBean &bean);

    void startPreviewTransition(SkyResourceBean &bean);

    void stopPreviewTransition();

    void stopPreviewEffect();

    SkyResourceBean m_previewResource;
    SkyResourceBean m_preTransitionResource;
    QModelIndex m_preTransitionResourceIndex;
    double lastPreviewTime = -1;
    SkyVideoTrack *m_skyTrack = nullptr;
};

#endif // STORERESOURCEPREVIEWCONTROLLER_H
