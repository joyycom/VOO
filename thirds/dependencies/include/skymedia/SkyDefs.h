#pragma once

#include <cstdint>
#include <cstring>

#if defined(_MSC_VER)
#define SKY_DEPRECATED
#define SKY_FORMAT_PRINTF(a, b)
#ifdef SKYMEDIA_EXPORTS
#define SKYMEDIA_API __declspec(dllexport)
#else
#define SKYMEDIA_API __declspec(dllimport)
#endif
#else
#define SKY_DEPRECATED __attribute__((deprecated))
#define SKY_FORMAT_PRINTF(a, b) __attribute__((format(printf, a, b)));
#define SKYMEDIA_API __attribute__((visibility("default")))
#endif

#define SKYMEDIA_NAMESPACE skymedia
#define SKYMEDIA_NAMESPACE_BEGIN namespace skymedia {
#define SKYMEDIA_NAMESPACE_END }
#define SKYMEDIA_USING_NAMESPACE using namespace skymedia;

SKYMEDIA_NAMESPACE_BEGIN

class SKYMEDIA_API SkyString final {
public:
    SkyString();
    SkyString(const char *str);
    SkyString(const SkyString &rhs);
    SkyString &operator=(const SkyString &rhs);
    ~SkyString();

    static SkyString loadFromFile(const char *filePath);

    const char *c_str() const;
    int size() const;

private:
    void *m_impl;
};

class SKYMEDIA_API SkyPath final {
public:
    SkyPath();
    SkyPath(const char *str);
    SkyPath(const SkyPath &rhs);
    SkyPath &operator=(const SkyPath &rhs);
    ~SkyPath();

    SkyPath join(const SkyPath &component) const;
    SkyPath parent() const;

    bool isExists() const;
    bool isFile() const;
    bool isDirectory() const;

    const char *c_str() const;
    int size() const;

private:
    void *m_impl;
};

class SKYMEDIA_API SkyStringList final {
public:
    SkyStringList();
    SkyStringList(const SkyStringList &rhs);
    SkyStringList &operator=(const SkyStringList &rhs);
    ~SkyStringList();

    int size() const;
    const char *c_strAt(int index) const;
    void append(const char *str);

private:
    void *m_impl;
};

struct SKYMEDIA_API SkyHash final {
    bool operator<(const SkyHash &rhs) const {
        return memcmp(data, rhs.data, 16) < 0;
    }

    bool operator==(const SkyHash &rhs) const {
        return memcmp(data, rhs.data, 16) == 0;
    }

    bool operator!=(const SkyHash &rhs) const {
        return !(*this == rhs);
    }
    uint8_t data[16] = {0}; // 初始清 0
};

struct SKYMEDIA_API SkyRect final {
    SkyRect() {
        x = 0.0;
        y = 0.0;
        width = 0.0;
        height = 0.0;
    }

    SkyRect(double x_, double y_, double w, double h) {
        x = x_;
        y = y_;
        width = w;
        height = h;
    }

    double x;
    double y;
    double width;
    double height;
};

struct SKYMEDIA_API SkyVec2D final {
    SkyVec2D() {
        x = 0.0;
        y = 0.0;
    }

    SkyVec2D(double x_, double y_) {
        x = x_;
        y = y_;
    }

    double x;
    double y;
};

struct SKYMEDIA_API SkyColor final {
    SkyColor() {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }

    SkyColor(double r_, double g_, double b_, double a_) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }

    double r;
    double g;
    double b;
    double a;
};

struct SKYMEDIA_API SkyTimeRange final {
    SkyTimeRange(double begin_, double end_) : begin(begin_), end(end_) {
    }

    SkyTimeRange() : begin(0), end(0) {
    }

    double begin;
    double end;
};

struct SKYMEDIA_API SkyTransform2D {
    SkyVec2D scale = {1.0, 1.0};
    double rotation = 0.0; // 旋转角度，弧度制
    SkyVec2D translation = {0.0, 0.0};
};

extern "C" SKYMEDIA_API double sky_cubicBezierXToY(double startX,
                                                   double startY,
                                                   double ctrl0X,
                                                   double ctrl0Y,
                                                   double ctrl1X,
                                                   double ctrl1Y,
                                                   double endX,
                                                   double endY,
                                                   double x);
SKYMEDIA_NAMESPACE_END
