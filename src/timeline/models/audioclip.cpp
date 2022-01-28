#include "audioclip.h"
#include "baseclip.h"
#include <skymedia/SkyTimeline.h>
#include <QMap>

SKYMEDIA_USING_NAMESPACE

AudioClip::AudioClip(const SkyResourceBean &bean, SkyClip *clip) : BaseClip(bean, clip) {
}

void AudioClip::onSaveInstanceState(QMap<QString, SkyVariant> &bundle) {
    BaseClip::onSaveInstanceState(bundle);
}

void AudioClip::onRestoreInstanceState(QMap<QString, SkyVariant> &bundle) {
    BaseClip::onRestoreInstanceState(bundle);
}


double AudioClip::getPlayPTS(double sourcePts) {
    return startTime() + sourcePts;
}

double AudioClip::getSourcePTS(double playPts) {
    return playPts - startTime();
}
