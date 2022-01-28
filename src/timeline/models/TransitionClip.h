#ifndef TRANSITIONCLIP_H
#define TRANSITIONCLIP_H

#include "baseclip.h"

class TransitionClip : public BaseClip {
public:
    TransitionClip(const SkyResourceBean &bean, SkyTransition *transition);

    ~TransitionClip();

    ClipType clipType() const override{
        return TransitionClipType;
    };

    SkyTransition *skyTransition();

    double during() const override;

    QString toString() const override;

    void setConfigTransition(float min,float max);

    double getMinTransition() const;
    double getMaxTransition() const;

private:

    SkyTransition *m_transition;
    float m_configMinTransition = 0.1;
    float m_configMaxTransition = 2;
};

#endif // TRANSITIONCLIP_H
