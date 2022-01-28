#include "MemoryPool.h"
#include "SkyVariantUtil.h"
#include "glew/glew.h"
#include "AppConstance.h"
#include <QDebug>

QString SkyVariantUtil::toString(const SkyVariant &variant) {
    switch (variant.getType()) {
        case SkyVariantType_Null:
            return "Null";
        case SkyVariantType_Bool:
            return QString(variant.toBool());
        case SkyVariantType_Double:
            return QString::number(variant.toDouble());
        case SkyVariantType_Int64:
            return QString::number(variant.toInt());
        case SkyVariantType_String:
            return variant.toString();
        default:
            return variant.transToJsonString(false).c_str();
    }
}

SkyVariant SkyVariantUtil::fromPointList(QList<QPointF> points) {
    SkyVariant array = SkyVariant::makeArray();
    for (QPointF p : points) {
        SkyVariant variant = SkyVariant::makeObject();
        variant.insert("x", p.x());
        variant.insert("y", p.y());
        array.append(variant);
    }
    return array;
}

QList<QPointF> SkyVariantUtil::toPointList(SkyVariant variant) {
    QList<QPointF> points;
    for (int i = 0; i < variant.size(); ++i) {
        SkyVariant item = variant[i];
        points.append(QPointF(item.getDouble("x", 0), item.getDouble("y", 0)));
    }
    return points;
}

QVariant SkyVariantUtil::toQVariant(SkyVariant variant) {
    switch (variant.getType()) {
        case SkyVariantType_Null:
            return QVariant();
        case SkyVariantType_Bool:
            return variant.toBool();
        case SkyVariantType_Double:
            return variant.toDouble();
        case SkyVariantType_Int64:
            return variant.toInt64();
        case SkyVariantType_String:
            return variant.toString();
        default:
            return QString("Unknown SkyVariant Type:%1").append(variant.getType());
    }
}

SkyVariant SkyVariantUtil::toSkyVariant(QVariant variant) {
    switch (variant.type()) {
        case QVariant::Bool:
            return variant.toBool();
        case QVariant::Int:
            return variant.toInt();
        case QVariant::Double:
            return variant.toDouble();
        case QVariant::LongLong:
            return variant.toLongLong();
        case QVariant::String:
            return variant.toString().toUtf8().data();
        default:
            return QString("Unknown Qvariant Type:%1").append(variant.typeName()).toUtf8().data();
    }
}

bool SkyVariantUtil::varEquals(SkyVariant left, SkyVariant right) {
    return left == right;
}

QColor SkyVariantUtil::variant2Color(SkyVariant colorRGB, QColor defColor) {
    QColor textColor = defColor;
    if (!colorRGB.isNull() && colorRGB.size() == 4) {
        textColor.setRedF(colorRGB.at(0).toDouble());
        textColor.setGreenF(colorRGB.at(1).toDouble());
        textColor.setBlueF(colorRGB.at(2).toDouble());
        textColor.setAlphaF(colorRGB.at(3).toDouble());
    }
    return textColor;
}

SkyVariant SkyVariantUtil::color2Variant(QColor c) {
    SkyVariant rgb = SkyVariant::makeArray();
    rgb.append(c.redF());
    rgb.append(c.greenF());
    rgb.append(c.blueF());
    rgb.append(c.alphaF());
    return rgb;
}
