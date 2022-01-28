#include "effectclip.h"

#include <QFileInfo>
#include <QDebug>

EffectClip::EffectClip(const SkyResourceBean &bean, SkyClip *clip) : BaseClip(bean, clip) {
    auto wrapper = skyEffectWrapper();
    if (wrapper) {
        auto text = wrapper->getOfParamValue("0:Text");
        if (!text.isNull()) {
            onWordContentChange(text.toString());
        }
    }
}

EffectClip::~EffectClip() {
}

void EffectClip::onSaveInstanceState(QMap<QString, SkyVariant> &bundle) {
    BaseClip::onSaveInstanceState(bundle);
}

void EffectClip::onRestoreInstanceState(QMap<QString, SkyVariant> &bundle) {
    BaseClip::onRestoreInstanceState(bundle);
    auto entryFile = QFileInfo(m_resBean.entryPath());
    if(!entryFile.isFile() || !entryFile.exists()){
        if(skyEffectWrapper() != nullptr){
            m_resBean = skyEffectWrapper()->skyResource();
            qDebug()<<" Use Default Effect Resource!"<<m_resBean.toString();
        }
    }
}

SkyEffect *EffectClip::addDefaultEffect() {
    if (!m_resBean.isValidResource()) {
        return nullptr;
    }
    SkyEffectWrapper *wraper = addEffect(m_resBean);
    if (wraper != nullptr) {
        return wraper->skyEffect();
    }
    return nullptr;
}

SkyEffectWrapper *EffectClip::skyEffectWrapper() const{
    if (clipType() == EffectClipType && m_effectList.size() > 0) {
        return m_effectList[0];
    }

    return nullptr;
}

double EffectClip::getPlayPTS(double sourcePts) {
    return startTime() + sourcePts;
}

double EffectClip::getSourcePTS(double playPts) {
    return playPts - startTime();
}
