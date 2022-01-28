#pragma once

#include <QObject>
#include "TransitionClip.h"
#include "baseclip.h"

class VideoClip : public BaseClip {
    Q_OBJECT
public:
    VideoClip(const SkyResourceBean &bean, SkyClip *clip = nullptr);

    ~VideoClip();

    ClipType clipType() const override {
        return VideoClipType;
    };

    QVariant get(int role) const override;

    SkyEffect *addDefaultEffect() override;

    SkyEffectWrapper* skyEffectWrapper() const override;

    bool addTransition(SkyResourceBean &resource, double during = -1);

    bool removeTransition();

    TransitionClip *getTransition();

    double transitionDuring() const;

    void setTransitionDuring(double during);

    QJsonDocument getUIConfig() override;

    bool rewindVideoClip(QString rewindPath);

    void updateVideoOverlayEffectScaleParam();

protected:
    virtual void onSaveInstanceState(QMap<QString, SkyVariant> &bundle) override;

    virtual void onRestoreInstanceState(QMap<QString, SkyVariant> &bundle) override;

private:
    double getMinTransition() const;
    double getMaxTransition() const;
    void setVideoOverlayEffectScaleParam(SkyEffectWrapper* wrapper);


    TransitionClip *m_transitionClip = nullptr;
    QString resourcePath;
    int thumbnailFrameIndex = 0;
};
