#include "imageclip.h"
#include <QDebug>
#include <QFile>
#include <QImage>
#include "TimelineDef.h"

ImageClip::ImageClip(const SkyResourceBean &bean, SkyClip *clip) : VideoClip(bean, clip) {
}

double ImageClip::resouceTime() const {
    return during();
}

double ImageClip::getPlayPTS(double sourcePts) {
    return startTime() + sourcePts;
}

double ImageClip::getSourcePTS(double playPts) {
    return playPts - startTime();
}
