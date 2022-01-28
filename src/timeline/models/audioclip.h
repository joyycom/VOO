#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H
#include "baseclip.h"

class AudioClip : public BaseClip {
public:
    AudioClip(const SkyResourceBean &bean, SkyClip *clip);

    virtual ClipType clipType() const override {
        return AudioClipType;
    };

    virtual double getPlayPTS(double sourcePts) override;

    virtual double getSourcePTS(double playPts) override;

protected:
    virtual void onSaveInstanceState(QMap<QString, SkyVariant> &bundle) override;

    virtual void onRestoreInstanceState(QMap<QString, SkyVariant> &bundle) override;
};

#endif // AUDIOCLIP_H
