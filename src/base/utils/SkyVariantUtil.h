#ifndef SKYVARIANTUTIL_H
#define SKYVARIANTUTIL_H
#include <skymedia/SkyVariant.h>
#include <QVariant>
#include <QPointF>
#include <QColor>
SKYMEDIA_USING_NAMESPACE
class SkyVariantUtil {

public:
    static bool equals(const SkyVariant &l, const SkyVariant &r);

    static QString toString(const SkyVariant &variant);

    static SkyVariant fromPointList(QList<QPointF> points);

    static QList<QPointF> toPointList(SkyVariant variant);

    static QVariant toQVariant(SkyVariant variant);

    static SkyVariant toSkyVariant(QVariant variant);

    static bool varEquals(SkyVariant left, SkyVariant right);

    static QColor variant2Color(SkyVariant colorRGB, QColor defColor);

    static SkyVariant color2Variant(QColor c);

};

#endif // SKYVARIANTUTIL_H
