#pragma once

#include "SkyEffectWrapper.h"
#include "baseclip.h"
#include <QFile>
#include <QObject>
#include <skymedia/SkyEffect.h>
SKYMEDIA_USING_NAMESPACE

class EffectClip : public BaseClip {
    Q_OBJECT
public:
    EffectClip(const SkyResourceBean &bean, SkyClip *clip = nullptr);

    ~EffectClip();

    virtual ClipType clipType() const override {
        return EffectClipType;
    };

    virtual SkyEffect *addDefaultEffect() override;

    virtual SkyEffectWrapper *skyEffectWrapper() const override;

    virtual double getPlayPTS(double sourcePts) override;
    virtual double getSourcePTS(double playPts) override;

protected:
    virtual void onSaveInstanceState(QMap<QString, SkyVariant> &bundle) override;

    virtual void onRestoreInstanceState(QMap<QString, SkyVariant> &bundle) override;

private:
};
