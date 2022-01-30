#pragma once

#include "SkyFrame.h"
#include "SkyVariant.h"

SKYMEDIA_NAMESPACE_BEGIN

struct SkyCurveItem final {
    double time = 0;
    SkyVariant value;
    char tweenName[32] = {0};
};

class SKYMEDIA_API SkyCurve final {
public:
    SkyCurve();
    SkyCurve(const SkyCurve &rhs);
    SkyCurve &operator=(const SkyCurve &rhs);
    ~SkyCurve();

    bool isEmpty() const;
    int numberOfItems() const;

    SkyCurveItem getItemAt(int index) const;

    void removeItemAt(int index);
    void removeItemAtTime(double time);

    int setItem(const SkyCurveItem &item);
    int setItem(double time, const SkyVariant &value, const char *tweenName);

    SkyVariant evaluate(double t) const;
    void offsetTime(double offsetT);

    SkyVariant saveConfig() const;
    void loadConfig(const SkyVariant &v);

    SkyVideoFrame debug_draw(int width, int height) const;

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
