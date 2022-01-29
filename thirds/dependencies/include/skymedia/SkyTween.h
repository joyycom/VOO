#pragma once

#include <math.h>

#ifdef __cplusplus
#define SKYMEDIA_C_EXTEN extern "C"
#else
#define SKYMEDIA_C_EXTEN
#endif

static const double sky_pi = 3.1415926535897932384626433832795;
static const double sky_pi2 = sky_pi / 2;

SKYMEDIA_C_EXTEN double sky_cubicBezierXToY(double startX,
                                            double startY,
                                            double ctrl0X,
                                            double ctrl0Y,
                                            double ctrl1X,
                                            double ctrl1Y,
                                            double endX,
                                            double endY,
                                            double x);

static inline double sky_linear(double p) {
    return p;
}

static inline double sky_cubicBezier(double p) {
    double ctrl0X = 0.5;
    double ctrl0Y = 0.0;
    double ctrl1X = 0.5;
    double ctrl1Y = 1.0;
    return sky_cubicBezierXToY(0.0, 0.0, ctrl0X, ctrl0Y, ctrl1X, ctrl1Y, 1.0, 1.0, p);
}

static inline double sky_none(double p) {
    return 0;
}

static inline double sky_full(double p) {
    return 1;
}

static inline double sky_noneLinear(double p) {
    if (p <= 0.5) {
        return 0.0;
    } else {
        return (p - 0.5) * 2.0;
    }
}

static inline double sky_linearFull(double p) {
    if (p <= 0.5) {
        return p * 2.0;
    } else {
        return 1.0;
    }
}

static inline double sky_quad_easeIn(double p) {
    return p * p;
}

static inline double sky_quad_easeOut(double p) {
    return -(p * (p - 2));
}

static inline double sky_quad_easeInOut(double p) {
    if (p < 0.5) {
        return 2 * p * p;
    } else {
        return (-2 * p * p) + (4 * p) - 1;
    }
}

static inline double sky_cubic_easeIn(double p) {
    return p * p * p;
}

static inline double sky_cubic_easeOut(double p) {
    double f = p - 1;
    return 1 + f * f * f;
}

static inline double sky_cubic_easeInOut(double p) {
    if (p < 0.5) {
        return 4 * p * p * p;
    } else {
        double f = ((2 * p) - 2);
        return 0.5 * f * f * f + 1;
    }
}

static inline double sky_quart_easeIn(double p) {
    return p * p * p * p;
}

static inline double sky_quart_easeOut(double p) {
    double f = (p - 1);
    return f * f * f * (1 - p) + 1;
}

static inline double sky_quart_easeInOut(double p) {
    if (p < 0.5) {
        return 8 * p * p * p * p;
    } else {
        double f = (p - 1);
        return -8 * f * f * f * f + 1;
    }
}

static inline double sky_quint_easeIn(double p) {
    return p * p * p * p * p;
}

static inline double sky_quint_easeOut(double p) {
    double f = (p - 1);
    return f * f * f * f * f + 1;
}

static inline double sky_quint_easeInOut(double p) {
    if (p < 0.5) {
        return 16 * p * p * p * p * p;
    } else {
        double f = ((2 * p) - 2);
        return 0.5 * f * f * f * f * f + 1;
    }
}

static inline double sky_sine_easeIn(double p) {
    return sin((p - 1) * sky_pi2) + 1;
}

static inline double sky_sine_easeOut(double p) {
    return sin(p * sky_pi2);
}

static inline double sky_sine_easeInOut(double p) {
    return 0.5 * (1 - cos(p * sky_pi));
}

static inline double sky_circ_easeIn(double p) {
    return 1 - sqrt(1 - (p * p));
}

static inline double sky_circ_easeOut(double p) {
    return sqrt((2 - p) * p);
}

static inline double sky_circ_easeInOut(double p) {
    if (p < 0.5) {
        return 0.5 * (1 - sqrt(1 - 4 * (p * p)));
    } else {
        return 0.5 * (sqrt(-((2 * p) - 3) * ((2 * p) - 1)) + 1);
    }
}

static inline double sky_expo_easeIn(double p) {
    return (p == 0.0) ? p : pow(2, 10 * (p - 1));
}

static inline double sky_expo_easeOut(double p) {
    return (p == 1.0) ? p : 1 - pow(2, -10 * p);
}

static inline double sky_expo_easeInOut(double p) {
    if (p == 0.0 || p == 1.0) {
        return p;
    }
    if (p < 0.5) {
        return 0.5 * pow(2, (20 * p) - 10);
    } else {
        return -0.5 * pow(2, (-20 * p) + 10) + 1;
    }
}

static inline double sky_elastic_easeIn(double p) {
    return sin(13 * sky_pi2 * p) * pow(2, 10 * (p - 1));
}

static inline double sky_elastic_easeOut(double p) {
    return sin(-13 * sky_pi2 * (p + 1)) * pow(2, -10 * p) + 1;
}

static inline double sky_elastic_easeInOut(double p) {
    if (p < 0.5) {
        return 0.5 * sin(13 * sky_pi2 * (2 * p)) * pow(2, 10 * ((2 * p) - 1));
    } else {
        return 0.5 * (sin(-13 * sky_pi2 * ((2 * p - 1) + 1)) * pow(2, -10 * (2 * p - 1)) + 2);
    }
}

static inline double sky_back_easeIn(double p) {
    double s = 1.70158f;
    return p * p * ((s + 1) * p - s);
}

static inline double sky_back_easeOut(double p) {
    double s = 1.70158f;
    --p;
    return 1.f * (p * p * ((s + 1) * p + s) + 1);
}

static inline double sky_back_easeInOut(double p) {
    double s = 1.70158f * 1.525f;
    if (p < 0.5) {
        p *= 2;
        return 0.5 * p * p * (p * s + p - s);
    } else {
        p = p * 2 - 2;
        return 0.5 * (2 + p * p * (p * s + p + s));
    }
}

static inline double sky_bounce_easeOut(double p) {
    if (p < 4.0 / 11.0) {
        return (121.0 * p * p) / 16.0;
    } else if (p < 8.0 / 11.0) {
        return (363.0 / 40.0 * p * p) - (99.0 / 10.0 * p) + 17.0 / 5.0;
    } else if (p < 9.0 / 10.0) {
        return (4356.0 / 361.0 * p * p) - (35442.0 / 1805.0 * p) + 16061.0 / 1805.0;
    } else {
        return (54.0 / 5.0 * p * p) - (513.0 / 25.0 * p) + 268.0 / 25.0;
    }
}

static inline double sky_bounce_easeIn(double p) {
    return 1.0 - sky_bounce_easeOut(1.0 - p);
}

static inline double sky_bounce_easeInOut(double p) {
    if (p < 0.5) {
        return 0.5 * (1 - sky_bounce_easeOut(1 - p * 2));
    } else {
        return 0.5 * sky_bounce_easeOut(p * 2 - 1) + 0.5;
    }
}

static struct {
    const char *name;
    double (*formula)(double t);
} skyBuiltInTweenTable[] = {
    {"linear", sky_linear},
    {"bezier", sky_cubicBezier},
    {"none", sky_none},
    {"full", sky_full},
    {"noneLinear", sky_noneLinear},
    {"linearFull", sky_linearFull},

    {"quad.easeIn", sky_quad_easeIn},
    {"quad.easeOut", sky_quad_easeOut},
    {"quad.easeInOut", sky_quad_easeInOut},

    {"cubic.easeIn", sky_cubic_easeIn},
    {"cubic.easeOut", sky_cubic_easeOut},
    {"cubic.easeInOut", sky_cubic_easeInOut},

    {"quart.easeIn", sky_quart_easeIn},
    {"quart.easeOut", sky_quart_easeOut},
    {"quart.easeInOut", sky_quart_easeInOut},

    {"quint.easeIn", sky_quint_easeIn},
    {"quint.easeOut", sky_quint_easeOut},
    {"quint.easeInOut", sky_quint_easeInOut},

    {"sine.easeIn", sky_sine_easeIn},
    {"sine.easeOut", sky_sine_easeOut},
    {"sine.easeInOut", sky_sine_easeInOut},

    {"circ.easeIn", sky_circ_easeIn},
    {"circ.easeOut", sky_circ_easeOut},
    {"circ.easeInOut", sky_circ_easeInOut},

    {"expo.easeIn", sky_expo_easeIn},
    {"expo.easeOut", sky_expo_easeOut},
    {"expo.easeInOut", sky_expo_easeInOut},

    {"elastic.easeIn", sky_elastic_easeIn},
    {"elastic.easeOut", sky_elastic_easeOut},
    {"elastic.easeInOut", sky_elastic_easeInOut},

    {"back.easeIn", sky_back_easeIn},
    {"back.easeOut", sky_back_easeOut},
    {"back.easeInOut", sky_back_easeInOut},

    {"bounce.easeIn", sky_bounce_easeIn},
    {"bounce.easeOut", sky_bounce_easeOut},
    {"bounce.easeInOut", sky_bounce_easeInOut},
};
