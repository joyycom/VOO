#include "TransitionClip.h"

#include <base/utils/JsonUtils.h>

#include <QDebug>
#include <qjsonvalue.h>
#include <QJsonArray>

#include "src/timeline/controller/components/TransitionComponent.h"

TransitionClip::TransitionClip(const SkyResourceBean &bean, SkyTransition *transition)
    : BaseClip(bean, nullptr),
      m_transition(transition) {

}

TransitionClip::~TransitionClip() {
    if (m_transition != nullptr) {
        m_transition->removeFromOwner();
        m_transition = nullptr;
        qDebug() << "TransitionClip Remove Transition!";
    }
}

SkyTransition *TransitionClip::skyTransition() {
    return m_transition;
}

QString TransitionClip::toString() const {
    return "TransitionClip: Resource:" + resourceBean().toString();
}

double TransitionClip::getMinTransition() const {
    return m_configMinTransition;
}

double TransitionClip::getMaxTransition() const {
    return m_configMaxTransition;
}

void TransitionClip::setConfigTransition(float min, float max){
    m_configMaxTransition = max;
    m_configMinTransition = min;
}

double TransitionClip::during() const {
    return m_transition != nullptr ? m_transition->getDuration() : 0;
}
