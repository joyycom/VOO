#pragma once

#include "baseclip.h"
#include "videoclip.h"
#include <QObject>

class ImageClip : public VideoClip {
public:
    ImageClip(const SkyResourceBean &bean, SkyClip *parent);

    virtual double resouceTime() const override;

    virtual ClipType clipType() const override {
        return ImageClipType;
    };

    virtual double getPlayPTS(double sourcePts) override;
    virtual double getSourcePTS(double playPts) override;
};
