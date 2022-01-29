/***************** BEGIN FILE HRADER BLOCK *********************************
 *
 * \author Cheng Yu <chengyu@yy.com> phone:13710201865
 *
 * Copyright (C) 2016-2020 JOYY Inc.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation; either version 3 of the License, or (at 
 * your option) any later version. Please review the following information 
 * to ensure the GNU Lesser General Public License version 3 requirements
 * will be met: https://www.gnu.org/licenses/lgpl.html.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation.
 *
 * If you use the source in a your project, please consider send an e-mail
 * to me, and allow me to add your project to my home page.
 *
 ***************** END FILE HRADER BLOCK ***********************************/

#pragma once

#include <cmath>
#include <memory>
#include <string.h>
#include <stdlib.h>
#include <cassert>

#if defined(__ARM_NEON__) && defined(__aarch64__) && defined(OF_IOS)
#include <arm_neon.h>
#endif
#include "of_core.h"

namespace OrangeFilter 
{

// Constants
const OFUInt32 MaxUInt32 = 0xFFFFFFFF;
const OFInt32 MinInt32   = 0x80000000;
const OFInt32 MaxInt32   = 0x7FFFFFFF;
const float MaxFloat     = 3.402823466e+38F;
const float MinPosFloat  = 1.175494351e-38F;
    
const float Pi     = 3.141592654f;
const float TwoPi  = 6.283185307f;
const float PiHalf = 1.570796327f;

const float Epsilon = 0.000001f;
const float ZeroEpsilon = 32.0f * MinPosFloat;  // Very small epsilon for checking against 0.0f
#ifdef __GNUC__
const float NaN = __builtin_nanf("");
#else
const float NaN = *(float *)&MaxUInt32;
#endif

enum NoInitHint
{
    NO_INIT
};

// -------------------------------------------------------------------------------------------------
// General
// -------------------------------------------------------------------------------------------------

static inline float degToRad(float f)
{
    return f * 0.017453293f;
}

static inline float radToDeg(float f)
{
    return f * 57.29577951f;
}

static inline float clamp(float f, float min, float max)
{
    if (f < min)
    {
        f = min;
    }
    else if (f > max)
    {
        f = max;
    }

    return f;
}

static inline float lerp(float from, float to, float t)
{
    return from + (to - from) * t;
}

static inline bool isZero(float f)
{
    return f < ZeroEpsilon && f > -ZeroEpsilon;
}

template <class T>
inline T RandomRange(T min, T max) // [min, max)
{
    double r = rand() / ((double)RAND_MAX + 1);
    return (T)(min + r * (max - min));
}

//
// Max.
//
template<class T>
inline const T& Max(const T& iV0, const T& iV1)
{
    return (iV0 < iV1) ? iV1 : iV0;
}

template<class T>
inline const T&  Max(const T& iV0, const T& iV1, const T& iV2)
{
    return Max(Max(iV0, iV1), iV2);
}

//
// Min.
//
template<class T>
inline const T& Min(const T& iV0, const T& iV1)
{
    return (iV0 > iV1) ? iV1 : iV0;
}

template<class T>
inline const T&  Min(const T& iV0, const T& iV1, const T& iV2)
{
    return Min(Min(iV0, iV1), iV2);
}

//
// Abs.
//
template <class T>
inline T Abs(T a)
{
    return a >= (T)0 ? a : -a;
}

inline float Abs(float a)
{
    return fabs(a);
}

inline double Abs(double a)
{
    return fabs(a);
}

template <class T>
inline T Square(const T &a)
{
    return a * a;
}

template <class T>
inline OFInt32 Sign(T a)
{
    return a < 0 ? -1 : 1;
}

template <class T>
inline void Swap(T& a, T& b)
{
    T temp = a; a = b; b = temp;
}

//
// Bound in [tLow, tHigh]
//
template<class T>
inline T Clamp(const T& t, const T& tLow, const T& tHigh)
{
    if (t < tHigh)
    {
        return ((t > tLow) ? t : tLow);
    }
    return tHigh;
}

inline OFUInt8 Clamp0255(OFInt32 n)
{
    return (OFUInt8)(OFInt32)Clamp(n, 0, 0xFF);
}

static inline float fsel(float test, float a, float b)
{
    //
    // Branchless selection.
    //
    return test >= 0 ? a : b;
}

// -------------------------------------------------------------------------------------------------
// Conversion
// -------------------------------------------------------------------------------------------------

static inline OFInt32 ftoi_t(double val)
{
    //
    // Float to OFInt32 conversion using truncation
    //
    return (OFInt32)val;
}

static inline OFInt32 ftoi_r(double val)
{
    //
    // Fast round (banker's round) using Sree Kotay's method
    // This function is much faster than a naive cast from float to OFInt32
    //
    union
    {
        double dval;
        OFInt32 ival[2];
    } u;

    u.dval = val + 6755399441055744.0;  // Magic number: 2^52 * 1.5;
    return u.ival[0];         // Needs to be [1] for big-endian
}

inline bool IsFloatEqual(double x, double y, double tol = 0.000000001)
{
    bool bRet = false;
    if (Abs(x - y) < tol)
    {
        bRet = true;
    }
    return bRet;
}

inline bool IsFloatZero(double val, double tol = 0.000000001)
{
    return IsFloatEqual(val, 0.0, tol);
}

inline bool IsFloatEqual(float x, float y, float tol = 0.0001)
{
    bool bRet = false;
    if (Abs(x - y) < tol)
    {
        bRet = true;
    }
    return bRet;
}

inline bool IsFloatZero(float val, float tol = 0.0001)
{
    return IsFloatEqual(val, (float)0.0, tol);
}

//
// Sqrt.
//
inline OFUInt32 U32Sqrt(OFUInt32 val)
{
    OFUInt32 r = 0;
    for (OFUInt32 shift = 0; shift<32; shift += 2)
    {
        OFInt32 x = 0x40000000l >> shift;
        if (x + r <= val)
        {
            val -= x + r;
            r = (r >> 1) | x;
        }
        else
        {
            r = r >> 1;
        }
    }

    return r;
};

// Reference: QUAKE3 by Carmack.
// http://openarena.ws/svn/source/code/game/q_math.c
// http://www.matrix67.com/data/InvSqrt.pdf (FAST INVERSE SQUARE ROOT)
inline float FSqrt(float number)
{
    long i;
    float x, y;
    const float f = 1.5F;
    x = number * 0.5F;
    y = number;
    i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (f - (x * y * y));
    y = y * (f - (x * y * y));
    return number * y;
};

void SqrtArr(const float* src, float* dst, OFInt32 len);

/* ************************************************************************** *\
Fast cube root by Ken Turkowski
(http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
inline float CubeRoot(float value)
{
    typedef union mixer32suf
    {
        OFInt32 i;
        OFUInt32 u;
        float f;
    }
    mixer32suf;

    float fr;
    mixer32suf v, m;
    OFInt32 ix, s;
    OFInt32 ex, shx;

    v.f = value;
    ix = v.i & 0x7fffffff;
    s = v.i & 0x80000000;
    ex = (ix >> 23) - 127;
    shx = ex % 3;
    shx -= shx >= 0 ? 3 : 0;
    ex = (ex - shx) / 3; /* exponent of cube root */
    v.i = (ix & ((1 << 23) - 1)) | ((shx + 127) << 23);
    fr = v.f;

    /* 0.125 <= fr < 1.0 */
    /* Use quartic rational polynomial with error < 2^(-24) */
    fr = (float)(((((45.2548339756803022511987494 * fr +
        192.2798368355061050458134625) * fr +
        119.1654824285581628956914143) * fr +
        13.43250139086239872172837314) * fr +
        0.1636161226585754240958355063) /
        ((((14.80884093219134573786480845 * fr +
        151.9714051044435648658557668) * fr +
        168.5254414101568283957668343) * fr +
        33.9905941350215598754191872) * fr +
        1.0));

    /* fr *= 2^ex * sign */
    m.f = value;
    v.f = fr;
    v.i = (v.i + (ex << 23) + s) & (m.i * 2 != 0 ? -1 : 0);
    return v.f;
}

inline OFInt32 Round(double value)
{
#if (defined _MSC_VER && defined _M_X64)
    __m128d t = _mm_set_sd(value);
    return _mm_cvtsd_si32(t);
#elif defined _MSC_VER && defined _M_IX86
    OFInt32 t;
    __asm
    {
        fld value;
        fistp t;
    }
    return t;
#else
    double intpart, fractpart;
    fractpart = modf(value, &intpart);
    if ((fabs(fractpart) != 0.5) || ((((OFInt32)intpart) % 2) != 0))
    {
        return (OFInt32)(value + (value >= 0 ? 0.5 : -0.5));
    }
    else
    {
        return (OFInt32)intpart;
    }
#endif
}

template<class T>
inline bool containsInArray(const T& value, const T* array, OFUInt32 count)
{
    for (OFUInt32 i = 0; i < count; ++i)
    {
        if (array[ i ] == value)
        {
            return true;
        }
    }
    return false;
}

inline OFUInt16 FloatToHalf(float f)
{
    OFUInt32 fb = *(OFUInt32*)&f;
    OFInt32 s = (fb >> 31) & 0x1;
    OFInt32 e = (fb >> 23) & 0xff;

    if (255 == e)
    {
        assert(false);
    }
    if (e > 0)
    {
        e -= (1 << (8 - 1)) - 1;// e -= 127
        e += (1 << (5 - 1)) - 1;// e += 15
        if (e < 0)
        {
            e = 0;
        }
        else if (e >= 31)
        {
            e = 30;
        }
    }
    OFInt32 m = (fb >> 0) & 0x7fffff;
    m >>= (23 - 10);

    OFUInt16 h = 0;
    h |= s << 15;
    h |= e << 10;
    h |= m;

    return h;
}

inline float HalfToFloat(OFUInt16 h)
{
    OFInt32 s = (h >> 15) & 0x1;
    OFInt32 e = (h >> 10) & 0x1f;
    if (31 == e)
    {
        assert(!"half e == 31");
    }
    if (e > 0)
    {
        e -= (1 << (5 - 1)) - 1;// e -= 15
        e += (1 << (8 - 1)) - 1;// e += 127
        assert(e > 0 && e < 255);
    }
    OFInt32 m = (h >> 0) & 0x3ff;
    m <<= (23 - 10);

    OFUInt32 f = 0;
    f |= s << 31;
    f |= e << 23;
    f |= m;

    return *(float*)&f;
}


// -------------------------------------------------------------------------------------------------
// Vector
// -------------------------------------------------------------------------------------------------

class OF_CORE_API Vec3us
{
public:
    OFUInt16 x, y, z;

    // ------------
    // Constructors
    // ------------
    Vec3us() : x(0), y(0), z(0)
    {}

    explicit Vec3us(NoInitHint)
    {
        // Constructor without default initialization
    }

    Vec3us(const OFUInt16 x, const OFUInt16 y, const OFUInt16 z) : x(x), y(y), z(z)
    {}

    Vec3us(const Vec3us &v) : x(v.x), y(v.y), z(v.z)
    {}

    void set(const OFUInt16 x, const OFUInt16 y, const OFUInt16 z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // ------
    // Access
    // ------
    OFUInt16 &operator[](OFUInt32 index)
    {
        return *(&x + index);
    }

    const OFUInt16 &operator[](OFUInt32 index) const
    {
        return *(&x + index);
    }

    // -----------
    // Comparisons
    // -----------
    bool operator==(const Vec3us &v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator!=(const Vec3us &v) const
    {
        return x != v.x || y != v.y || z != v.z;
    }

    // ---------------------
    // Arithmetic operations
    // ---------------------
    Vec3us operator-() const
    {
        return Vec3us(-x, -y, -z);
    }

    Vec3us operator+(const Vec3us &v) const
    {
        return Vec3us(x + v.x, y + v.y, z + v.z);
    }

    Vec3us &operator+=(const Vec3us &v)
    {
        return *this = *this + v;
    }

    Vec3us operator-(const Vec3us &v) const
    {
        return Vec3us(x - v.x, y - v.y, z - v.z);
    }

    Vec3us &operator-=(const Vec3us &v)
    {
        return *this = *this - v;
    }

    Vec3us operator*(int f) const
    {
        return Vec3us(x * f, y * f, z * f);
    }

    Vec3us &operator*=(int f)
    {
        return *this = *this * f;
    }

    Vec3us operator/(int f) const
    {
        return Vec3us(x / f, y / f, z / f);
    }

    Vec3us &operator/=(int f)
    {
        return *this = *this / f;
    }
};

class OF_CORE_API Vec2us
{
public:
    OFUInt16 x, y;
    Vec2us() :
        x(0), y(0)
    {
    }
    Vec2us(OFUInt16 x, OFUInt16 y) :
        x(x), y(y)
    {
    }
};

class OF_CORE_API Vec2f
{
public:
    float x, y;
    
    // ------------
    // Constructors
    // ------------
    Vec2f() : x( 0.0f ), y( 0.0f )
    { 
    }

    explicit Vec2f( NoInitHint )
    {
        // Constructor without default initialization
    }
    
    Vec2f( const float x, const float y ) : x( x ), y( y )
    {
    }

    Vec2f( const Vec2f &v ) : x( v.x ), y( v.y )
    {
    }

    void set(const float x, const float y)
    {
        this->x = x;
        this->y = y;
    }

    // ------
    // Access
    // ------
    float &operator[]( OFUInt32 index )
    {
        return *(&x + index);
    }

    const float &operator[](OFUInt32 index) const
    {
        return *(&x + index);
    }
    
    // -----------
    // Comparisons
    // -----------
    bool operator==( const Vec2f &v ) const
    {
        return x == v.x && y == v.y;
    }

    bool operator!=( const Vec2f &v ) const
    {
        return x != v.x || y != v.y;
    }
    
    // ---------------------
    // Arithmetic operations
    // ---------------------
    Vec2f operator-() const
    {
        return Vec2f( -x, -y );
    }

    Vec2f operator+( const Vec2f &v ) const
    {
        return Vec2f( x + v.x, y + v.y);
    }

    Vec2f operator+(const float f) const
    {
        return Vec2f(x + f, y + f);
    }

    Vec2f &operator+=( const Vec2f &v )
    {
        return *this = *this + v;
    }

    Vec2f operator-( const Vec2f &v ) const 
    {
        return Vec2f( x - v.x, y - v.y);
    }

    Vec2f operator-(const float f) const
    {
        return Vec2f(x - f, y - f);
    }

    Vec2f &operator-=( const Vec2f &v )
    {
        return *this = *this - v;
    }

    Vec2f &operator-=(const float f)
    {
        return *this = *this - f;
    }

    Vec2f &operator+=(const float f)
    {
        return *this = *this + f;
    }

    Vec2f operator*( const float f ) const
    {
        return Vec2f( x * f, y * f );
    }

    Vec2f &operator*=( const float f )
    {
        return *this = *this * f;
    }

    Vec2f operator/( const float f ) const
    {
        return Vec2f( x / f, y / f );
    }

    Vec2f &operator/=( const float f )
    {
        return *this = *this / f;
    }

    friend Vec2f operator*(float t, Vec2f v)
    {
        Vec2f w(v);
        return (w *= t);
    }

    friend Vec2f operator/(float t, Vec2f v)
    {
        return Vec2f(t / v.x, t / v.y);
    }

    friend Vec2f operator+(float t, Vec2f v)
    {
        Vec2f w(v);
        return (w += t);
    }

    friend Vec2f operator-(float t, Vec2f v)
    {
        Vec2f w(v);
        return (w -= t);
    }

    Vec2f& operator=(const Vec2us& v)
    {
        this->x = HalfToFloat(v.x);
        this->y = HalfToFloat(v.y);
        return *this;
    }

    // ----------------
    // Special products
    // ----------------
    float dot( const Vec2f &v ) const
    {
        return x * v.x + y * v.y;
    }

    float cross(const Vec2f& v) const
    {
        return x * v.y - y * v.x;
    }

    // ----------------
    // Other operations
    // ----------------
    float length() const 
    {
        return sqrtf( x * x + y * y);
    }

    float sqrLength() const
    {
        return x * x + y * y;
    }

    Vec2f normalized() const
    {
        float invLen = 1.0f / length();
        return Vec2f( x * invLen, y * invLen );
    }

    void normalize()
    {
        float invLen = 1.0f / length();
        x *= invLen;
        y *= invLen;
    }

    Vec2f lerp( const Vec2f &v, float f ) const
    {
        return Vec2f( x + (v.x - x) * f, y + (v.y - y) * f ); 
    }

    void absolute()
    {
        if (x < 0)
        {
            x = -x;
        }
        if (y < 0)
        {
            y = -y;
        }
    }

    Vec2f square()
    {
        return Vec2f(x * x, y * y);
    }

    float radianBetween(const Vec2f& rhs)
    {
        return acos(dot(rhs) / (length() * rhs.length()));
    }

    bool isZero() const
    {
        return (fabs(x - 0) < 1e-3f) && (fabs(y - 0) < 1e-3f);
    }

    static float Angle(const Vec2f& from, const Vec2f& to)
    {
        float mod = from.sqrLength() * to.sqrLength();
        float dot = from.dot(to) / sqrt(mod);
        //dot = Clamp(dot, -1.0f, 1.0f);
        float deg = radToDeg(acos(dot));

        if (from.cross(to) < 0.0f)
        {
            deg = -deg;
        }

        return deg;
    }
};

class OF_CORE_API Vec3f
{
public:
    float x, y, z;
    
    
    // ------------
    // Constructors
    // ------------
    Vec3f() : x( 0.0f ), y( 0.0f ), z( 0.0f )
    { 
    }

    explicit Vec3f(NoInitHint)
    {
        // Constructor without default initialization
    }
    
    Vec3f( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) 
    {
    }

    Vec3f( const Vec3f &v ) : x( v.x ), y( v.y ), z( v.z )
    {
    }

    void set(const float x, const float y, const float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // ------
    // Access
    // ------
    float &operator[]( OFUInt32 index )
    {
        return *(&x + index);
    }

    const float &operator[](OFUInt32 index) const
    {
        return *(&x + index);
    }
    
    // -----------
    // Comparisons
    // -----------
    bool operator==( const Vec3f &v ) const
    {
        return IsFloatEqual(x, v.x) && IsFloatEqual(y, v.y) && IsFloatEqual(z, v.z);
    }

    bool operator!=( const Vec3f &v ) const
    {
        return !(*this == v);
    }
    
    // ---------------------
    // Arithmetic operations
    // ---------------------
    Vec3f operator-() const
    {
        return Vec3f( -x, -y, -z );
    }

    Vec3f operator+( const Vec3f &v ) const
    {
        return Vec3f( x + v.x, y + v.y, z + v.z );
    }

    Vec3f &operator+=( const Vec3f &v )
    {
        return *this = *this + v;
    }

    Vec3f operator-( const Vec3f &v ) const 
    {
        return Vec3f( x - v.x, y - v.y, z - v.z );
    }

    Vec3f &operator-=( const Vec3f &v )
    {
        return *this = *this - v;
    }

    Vec3f operator*( const float f ) const
    {
        return Vec3f( x * f, y * f, z * f );
    }

    Vec3f &operator*=( const float f )
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    Vec3f operator/( const float f ) const
    {
        return Vec3f( x / f, y / f, z / f );
    }

    Vec3f &operator/=( const float f )
    {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }

    Vec3f& operator=(const Vec3us& v)
    {
        this->x = HalfToFloat(v.x);
        this->y = HalfToFloat(v.y);
        this->z = HalfToFloat(v.z);
        return *this;
    }

    // ----------------
    // Special products
    // ----------------
    float dot( const Vec3f &v ) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    Vec3f cross( const Vec3f &v ) const
    {
        return Vec3f( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
    }

    // ----------------
    // Other operations
    // ----------------
    float length() const 
    {
        return sqrtf( x * x + y * y + z * z );
    }

    float sqrLength() const
    {
        return x * x + y * y + z * z;
    }

    Vec3f normalized() const
    {
        float l = length();
        if (l < 0.000001f)
        {
            return Vec3f(0.0f, 0.0f, 0.0f);
        }
        float invLen = 1.0f / l;
        return Vec3f( x * invLen, y * invLen, z * invLen );
    }

    void normalize()
    {
        float l = length();
        if (l < 0.000001f)
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            return;
        }

        float invLen = 1.0f / l;
        x *= invLen;
        y *= invLen;
        z *= invLen;
    }

    static Vec3f Normalize(const Vec3f& v)
    {
        return v.normalized();
    }

    /*void fromRotation( float angleX, float angleY )
    {
        x = cosf( angleX ) * sinf( angleY ); 
        y = -sinf( angleX );
        z = cosf( angleX ) * cosf( angleY );
    }*/

    Vec3f toRotation() const
    {
        // Assumes that the unrotated view vector is (0, 0, -1)
        Vec3f v;
        
        if( y != 0 ) v.x = atan2f( y, sqrtf( x*x + z*z ) );
        if( x != 0 || z != 0 ) v.y = atan2f( -x, -z );

        return v;
    }

    Vec3f lerp( const Vec3f &v, float f ) const
    {
        return Vec3f( x + (v.x - x) * f, y + (v.y - y) * f, z + (v.z - z) * f ); 
    }

    static Vec3f Lerp(const Vec3f& a, const Vec3f& b, float f)
    {
        return a.lerp(b, f);
    }

    static float Angle(const Vec3f& from, const Vec3f& to)
    {
        float mod = from.sqrLength() * to.sqrLength();
        float dot = from.dot(to) / sqrt(mod);
        dot = Clamp(dot, -1.0f, 1.0f);
        
        float deg = radToDeg(acos(dot));

        return deg;
    }
};

class OF_CORE_API Vec4s
{
public:
    OFInt16 x, y, z, w;
    Vec4s() :
        x(0), y(0), z(0), w(0)
    {
    }
    Vec4s(OFInt16 x, OFInt16 y, OFInt16 z, OFInt16 w) :
        x(x), y(y), z(z), w(w)
    {
    }
};

// used as vec4 half
class OF_CORE_API Vec4us
{
public:
    OFUInt16 x, y, z, w;
    Vec4us() :
        x(0), y(0), z(0), w(0)
    {
    }
    Vec4us(OFUInt16 x, OFUInt16 y, OFUInt16 z, OFUInt16 w) :
        x(x), y(y), z(z), w(w)
    {
    }
};

class OF_CORE_API Vec4f
{
public:
    float x, y, z, w;

    Vec4f() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f )
    {
    }

    explicit Vec4f( const float x, const float y, const float z, const float w ) :
        x( x ), y( y ), z( z ), w( w )
    {
    }

    explicit Vec4f( Vec3f v ) : x( v.x ), y( v.y ), z( v.z ), w( 1.0f )
    {
    }

    Vec4f operator+( const Vec4f &v ) const
    {
        return Vec4f( x + v.x, y + v.y, z + v.z, w + v.w );
    }

    Vec4f operator-(const Vec4f &v) const
    {
        return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    Vec4f operator-() const
    {
        return Vec4f( -x, -y, -z, -w );
    }
    
    Vec4f operator*( const float f ) const
    {
        return Vec4f( x * f, y * f, z * f, w * f );
    }

    Vec4f operator/(const float f) const
    {
        return Vec4f(x / f, y / f, z / f, w / f);
    }

    bool operator==(const Vec4f& right) const
    {
        return 
            IsFloatEqual(x, right.x) &&
            IsFloatEqual(y, right.y) && 
            IsFloatEqual(z, right.z) && 
            IsFloatEqual(w, right.w);
    }

    bool operator!=(const Vec4f& right) const
    {
        return !(*this == right);
    }

    Vec4f& operator=(const Vec4us& v)
    {
        this->x = HalfToFloat(v.x);
        this->y = HalfToFloat(v.y);
        this->z = HalfToFloat(v.z);
        this->w = HalfToFloat(v.w);
        return *this;
    }

    void set(const float x, const float y, const float z, const float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
};

class OF_CORE_API Vec2i
{
public:
    OFInt32 x, y;

    Vec2i():
        x(0), y(0)
    {
    }

    Vec2i(OFInt32 x, OFInt32 y):
        x(x), y(y)
    {
    }
};


class OF_CORE_API Vec4i
{
public:
    OFInt32 x, y, z, w;

    Vec4i():
        x(0), y(0), z(0), w(0)
    {
    }

    Vec4i(OFInt32 x, OFInt32 y, OFInt32 z, OFInt32 w):
        x(x), y(y), z(z), w(w)
    {
    }
};

class OF_CORE_API Rectf
{
public:
    float x, y;
    float width, height;

    Rectf() : width(), height()
    {
    }

    Rectf(float w, float h) : x(), y(), width(w), height(h)
    {
    }

    Rectf(float startx, float starty, float w, float h) : x(startx), y(starty), width(w), height(h)
    {
    }

    bool isValid() const
    {
        return width > 0 && height > 0;
    }

    friend inline bool operator!=(const Rectf& lhs, const Rectf& rhs)
    {
        return !(lhs == rhs);
    }

    friend inline bool operator==(const Rectf& lhs, const Rectf& rhs)
    {
        return (fabs(lhs.x - rhs.x) < 1e-3f) &&
               (fabs(lhs.y - rhs.y) < 1e-3f) &&
               (fabs(lhs.width  - rhs.width)  < 1e-3f) &&
               (fabs(lhs.height - rhs.height) < 1e-3f);
    }

    friend inline Rectf operator*(const Rectf& lhs, const float& t)
    {
        return Rectf(lhs.x * t, lhs.y * t, lhs.width * t, lhs.height * t);
    }

    friend inline Rectf operator*(const float& t, const Rectf& rhs)
    {
        return rhs * t;
    }

    friend inline Rectf operator+(const Rectf& lhs, const Rectf& rhs)
    {
        return Rectf(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.width + rhs.width,
            lhs.height + rhs.height
        );
    }

    Rectf& operator*=(const float& t)
    {
        x *= t;
        y *= t;
        width  *= t;
        height *= t;

        return *this;
    }

    Rectf& operator+=(const Rectf& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        width  += rhs.width;
        height += rhs.height;

        return *this;
    }
};


// -------------------------------------------------------------------------------------------------
// Quaternion
// -------------------------------------------------------------------------------------------------

class OF_CORE_API Quaternion
{
public:
    union
    {
        float ptr[4];
        struct { float x, y, z, w; };
    };

    static const Quaternion& identity()
    {
        static Quaternion value(0.0f, 0.0f, 0.0f, 1.0f);
        return value;
    }

    // ------------
    // Constructors
    // ------------
    Quaternion() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) 
    { 
    }
    
    explicit Quaternion( const float x, const float y, const float z, const float w ) :
        x( x ), y( y ), z( z ), w( w )
    {
    }
    
    Quaternion( const float eulerX, const float eulerY, const float eulerZ )
    {
        Quaternion roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
        Quaternion pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
        Quaternion yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );
    
        // Order: y * x * z
        *this = pitch * roll * yaw;
    }

    // ---------------------
    // Arithmetic operations
    // ---------------------
    Quaternion operator*( const Quaternion &q ) const
    {
        return Quaternion(
            y * q.z - z * q.y + q.x * w + x * q.w,
            z * q.x - x * q.z + q.y * w + y * q.w,
            x * q.y - y * q.x + q.z * w + z * q.w,
            w * q.w - (x * q.x + y * q.y + z * q.z)
        );
    }

    Quaternion &operator*=( const Quaternion &q )
    {
        return *this = *this * q;
    }

    bool operator ==(const Quaternion &q)
    {
        return IsFloatEqual(x, q.x) && IsFloatEqual(y, q.y) && IsFloatEqual(z, q.z) && IsFloatEqual(w, q.w);
    }

    bool operator !=(const Quaternion &q)
    {
        return !(*this == q);
    }

    // ----------------
    // Other operations
    // ----------------

    Quaternion& operator=(const Vec4us& v)
    {
        this->x = HalfToFloat(v.x);
        this->y = HalfToFloat(v.y);
        this->z = HalfToFloat(v.z);
        this->w = HalfToFloat(v.w);
        return *this;
    }

    Quaternion slerp( const Quaternion &q, const float t ) const
    {
        // Spherical linear interpolation between two quaternions
        // Note: SLERP is not commutative
        
        Quaternion q1( q );

        // Calculate cosine
        float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;

        // Use the shortest path
        if( cosTheta < 0 )
        {
            cosTheta = -cosTheta; 
            q1.x = -q.x; q1.y = -q.y;
            q1.z = -q.z; q1.w = -q.w;
        }

        // Initialize with linear interpolation
        float scale0 = 1 - t, scale1 = t;
        
        // Use spherical interpolation only if the quaternions are not very close
        if( (1 - cosTheta) > 0.001f )
        {
            // SLERP
            float theta = acosf( cosTheta );
            float sinTheta = sinf( theta );
            scale0 = sinf( (1 - t) * theta ) / sinTheta;
            scale1 = sinf( t * theta ) / sinTheta;
        } 
        
        // Calculate final quaternion
        return Quaternion(
            x * scale0 + q1.x * scale1, y * scale0 + q1.y * scale1,
            z * scale0 + q1.z * scale1, w * scale0 + q1.w * scale1
        );
    }

    Quaternion nlerp( const Quaternion &q, const float t ) const
    {
        // Normalized linear quaternion interpolation
        // Note: NLERP is faster than SLERP and commutative but does not yield constant velocity

        Quaternion qt;
        float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;
        
        // Use the shortest path and interpolate linearly
        if (cosTheta < 0)
        {
            qt = Quaternion(
                x + (-q.x - x) * t, y + (-q.y - y) * t,
                z + (-q.z - z) * t, w + (-q.w - w) * t
            );
        }
        else
        {
            qt = Quaternion(
                x + (q.x - x) * t, y + (q.y - y) * t,
                z + (q.z - z) * t, w + (q.w - w) * t
            );
        }

        // Return normalized quaternion
        float invLen = 1.0f / sqrtf( qt.x * qt.x + qt.y * qt.y + qt.z * qt.z + qt.w * qt.w );
        return Quaternion( qt.x * invLen, qt.y * invLen, qt.z * invLen, qt.w * invLen );
    }

    Quaternion inverted() const
    {
        float len = x * x + y * y + z * z + w * w;
        if( len > 0 )
        {
            float invLen = 1.0f / len;
            return Quaternion( -x * invLen, -y * invLen, -z * invLen, w * invLen );
        }
        else return Quaternion();
    }

    Vec3f operator *(const Vec3f& p) const
    {
        Quaternion p_ = *this * Quaternion(p.x, p.y, p.z, 0) * this->inverted();

        return Vec3f(p_.x, p_.y, p_.z);
    }

    float dot(const Quaternion& v) const
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    float length() const
    {
        return sqrtf(x * x + y * y + z * z + w * w);
    }

    Quaternion normalized() const
    {
        float l = length();
        if (l < Epsilon)
        {
            return identity();
        }
        
        return Quaternion(x / l, y / l, z / l, w / l);
    }

    void normalize()
    {
        //*this = this->normalized();

        float l = length();
        if (l < Epsilon)
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            w = 1.0f;
        }
        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    static Quaternion AngleAxis(float angle, const Vec3f& axis)
    {
        Vec3f v = Vec3f::Normalize(axis);
        float cosv, sinv;
        
        cosv = cos(degToRad(angle * 0.5f));
        sinv = sin(degToRad(angle * 0.5f));

        float x = v.x * sinv;
        float y = v.y * sinv;
        float z = v.z * sinv;
        float w = cosv;

        return Quaternion(x, y, z, w);
    }

    static Quaternion FromToRotation(const Vec3f& from_direction, const Vec3f& to_direction)
    {
        Vec3f origin = Vec3f::Normalize(from_direction);
        Vec3f fn = Vec3f::Normalize(to_direction);

        if (fn != origin)
        {
            if (!IsFloatZero(fn.sqrLength()) && !IsFloatZero(origin.sqrLength()))
            {
                float deg = Vec3f::Angle(origin, fn);
                Vec3f axis = origin.cross(fn);
                
                if (axis == Vec3f(0, 0, 0))
                {
                    if (!IsFloatZero(origin.x))
                    {
                        float x = -origin.y / origin.x;
                        float y = 1;
                        float z = 0;

                        axis = Vec3f(x, y, z);
                    }
                    else if (!IsFloatZero(origin.y))
                    {
                        float y = -origin.z / origin.y;
                        float x = 0;
                        float z = 1;

                        axis = Vec3f(x, y, z);
                    }
                    else
                    {
                        float z = -origin.x / origin.z;
                        float y = 0;
                        float x = 1;

                        axis = Vec3f(x, y, z);
                    }

                    return AngleAxis(deg, axis);
                }
                else
                {
                    return AngleAxis(deg, axis);
                }
            }
        }

        return identity();
    }

    static Quaternion LookRotation(const Vec3f& forward, const Vec3f& up)
    {
        Vec3f un = up.normalized();
        Vec3f fn = forward.normalized();

        Quaternion rot0 = FromToRotation(Vec3f(0, 1, 0), un);
        Vec3f f = rot0 * Vec3f(0, 0, 1);
        float deg = Vec3f::Angle(f, fn);
        Quaternion rot1;
        Vec3f axis = f.cross(fn);
        float d = axis.dot(un);
        if (d > 0)
        {
            rot1 = AngleAxis(deg, up);
        }
        else
        {
            rot1 = AngleAxis(-deg, up);
        }
        return rot1 * rot0;
    }

    static Quaternion Euler(const Vec3f& euler)
    {
        return Quaternion(euler.x, euler.y, euler.z);
    }

    static Quaternion EulerRadians(float x, float y, float z)
    {
        return Quaternion(x, y, z);
    }

    static Quaternion EulerDegree(float x, float y, float z)
    {
        return Quaternion(degToRad(x), degToRad(y), degToRad(z));
    }

    Vec3f toEulerAngles() const
    {
        float rx = atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y));
        float ry = asin(2 * (w * y - z * x));
        float rz = atan2(2 * (w * z + x * y), 1 - 2 * (z * z + y * y));
        
        return Vec3f(radToDeg(rx), radToDeg(ry), radToDeg(rz));
    }
};


// -------------------------------------------------------------------------------------------------
// Matrix
// -------------------------------------------------------------------------------------------------

class OF_CORE_API Matrix2f
{
public:
    union
    {
        float c[2][2];
        float x[4];
    };

    Matrix2f()
    {
        c[0][0] = 1; c[1][0] = 0;
        c[0][1] = 0; c[1][1] = 1;
    }

    Matrix2f(float m00, float m01, float m10, float m11)
    {
        x[0] = m00; x[1] = m10;
        x[2] = m01; x[3] = m11;
    }

    Matrix2f(float* floatArray4)
    {
        for (OFUInt32 i = 0; i < 2; ++i)
        {
            for (OFUInt32 j = 0; j < 2; ++j)
            {
                c[i][j] = floatArray4[i * 2 + j];
            }
        }
    }

    Matrix2f(const Matrix2f& rhs)
    {
        assign(rhs);
    }

    Matrix2f& operator=(const Matrix2f& rhs)
    {
        assign(rhs);
        return *this;
    }

    void assign(const Matrix2f& rhs)
    {
        memcpy(x, rhs.x, sizeof(x));
    }

    friend bool operator!=(const Matrix2f& lhs, const Matrix2f& rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator==(const Matrix2f& lhs, const Matrix2f& rhs)
    {
        return (fabs(lhs.x[0] - rhs.x[0]) < 1e-3f)
            && (fabs(lhs.x[1] - rhs.x[1]) < 1e-3f)
            && (fabs(lhs.x[2] - rhs.x[2]) < 1e-3f)
            && (fabs(lhs.x[3] - rhs.x[3]) < 1e-3f);
    }

    friend inline Matrix2f operator*(const Matrix2f& lhs, float t)
    {
        return Matrix2f(
            lhs.x[0] * t, lhs.x[1] * t,
            lhs.x[2] * t, lhs.x[3] * t
        );
    }

    friend inline Matrix2f operator*(float t, const Matrix2f& rhs)
    {
        return rhs * t;
    }

    friend inline Matrix2f operator+(const Matrix2f& lhs, const Matrix2f& rhs)
    {
        return Matrix2f(
            lhs.x[0] + rhs.x[0], lhs.x[1] + rhs.x[1],
            lhs.x[2] + rhs.x[2], lhs.x[3] + rhs.x[3]
        );
    }

    friend inline Vec2f operator*(const Matrix2f& matrix, const Vec2f& vec)
    {
        return Vec2f(
            matrix.c[0][0] * vec.x + matrix.c[1][0] * vec.y,
            matrix.c[0][1] * vec.x + matrix.c[1][1] * vec.y
        );
    }

    friend inline Vec2f operator*(const Vec2f& vec, const Matrix2f& matrix)
    {
        return matrix * vec;
    }

    inline Matrix2f& operator*=(const float& t)
    {
        x[0] *= t; x[1] *= t;
        x[2] *= t; x[3] *= t;

        return *this;
    }

    inline Matrix2f& operator +=(const Matrix2f& rhs)
    {
        x[0] += rhs.x[0]; x[1] += rhs.x[1];
        x[2] += rhs.x[2]; x[3] += rhs.x[3];
        return *this;
    }

    inline Matrix2f transpose() const
    {
        return Matrix2f(
            c[0][0], c[1][0],
            c[0][1], c[1][1]
        );
    }

    const float* pointer() const
    {
        return &x[0];
    }
};

class OF_CORE_API Matrix4f
{
public:
    union
    {
        float c[4][4];    // Column major order for OpenGL: c[column][row]
        float x[16];
    };

    /** equals to a matrix full of zeros */
    static const Matrix4f ZERO;
    /** equals to the identity matrix */
    static const Matrix4f IDENTITY;
    /** equals to the reflect matrix */
    static const Matrix4f REFLECTMAT;

    static Matrix4f ReflectMat()
    {
        return REFLECTMAT;
    }

    static Matrix4f TransMat( float x, float y, float z )
    {
        Matrix4f m;

        m.c[3][0] = x;
        m.c[3][1] = y;
        m.c[3][2] = z;

        return m;
    }

    static Matrix4f ScaleMat( float x, float y, float z )
    {
        Matrix4f m;
        
        m.c[0][0] = x;
        m.c[1][1] = y;
        m.c[2][2] = z;

        return m;
    }

    static Matrix4f RotMat( float x, float y, float z ) // x, y, z in radians
    {
        // Rotation order: YXZ [* Vector]
        return Matrix4f( Quaternion( x, y, z ) );
    }

    static Matrix4f RotMat( Vec3f axis, float angle )
    {
        axis.normalize();
        axis *= sinf( angle * 0.5f );
        return Matrix4f( Quaternion( axis.x, axis.y, axis.z, cosf( angle * 0.5f ) ) );
    }

    static Matrix4f TRS(const Vec3f& translate, const Quaternion& rotate, const Vec3f& scale)
    {
        Matrix4f mt = TransMat(translate.x, translate.y, translate.z);
        Matrix4f mr = Matrix4f(rotate);
        Matrix4f ms = ScaleMat(scale.x, scale.y, scale.z);

        return mt * mr * ms;
    }

    static void LookAt(Matrix4f& result,
        float eyeX, float eyeY, float eyeZ,
        float targetX, float targetY, float targetZ,
        float upX, float upY, float upZ)
    {
        float axisX[ 3 ], axisY[ 3 ], axisZ[ 3 ];
        float length;

        // axisZ = lookAt - pos
        axisZ[ 0 ] = targetX - eyeX;
        axisZ[ 1 ] = targetY - eyeY;
        axisZ[ 2 ] = targetZ - eyeZ;

        // normalize axisZ
        length = sqrtf(axisZ[ 0 ] * axisZ[ 0 ] + axisZ[ 1 ] * axisZ[ 1 ] + axisZ[ 2 ] * axisZ[ 2 ]);

        if (length != 0.0f)
        {
            axisZ[ 0 ] /= length;
            axisZ[ 1 ] /= length;
            axisZ[ 2 ] /= length;
        }

        // axisX = up X axisZ
        axisX[ 0 ] = upY * axisZ[ 2 ] - upZ * axisZ[ 1 ];
        axisX[ 1 ] = upZ * axisZ[ 0 ] - upX * axisZ[ 2 ];
        axisX[ 2 ] = upX * axisZ[ 1 ] - upY * axisZ[ 0 ];

        // normalize axisX
        length = sqrtf(axisX[ 0 ] * axisX[ 0 ] + axisX[ 1 ] * axisX[ 1 ] + axisX[ 2 ] * axisX[ 2 ]);

        if (length != 0.0f)
        {
            axisX[ 0 ] /= length;
            axisX[ 1 ] /= length;
            axisX[ 2 ] /= length;
        }

        // axisY = axisZ x axisX
        axisY[ 0 ] = axisZ[ 1 ] * axisX[ 2 ] - axisZ[ 2 ] * axisX[ 1 ];
        axisY[ 1 ] = axisZ[ 2 ] * axisX[ 0 ] - axisZ[ 0 ] * axisX[ 2 ];
        axisY[ 2 ] = axisZ[ 0 ] * axisX[ 1 ] - axisZ[ 1 ] * axisX[ 0 ];

        // normalize axisY
        length = sqrtf(axisY[ 0 ] * axisY[ 0 ] + axisY[ 1 ] * axisY[ 1 ] + axisY[ 2 ] * axisY[ 2 ]);

        if (length != 0.0f)
        {
            axisY[ 0 ] /= length;
            axisY[ 1 ] /= length;
            axisY[ 2 ] /= length;
        }

        memset(&result, 0x0, sizeof(Matrix4f));

        result.c[ 0 ][ 0 ] = -axisX[ 0 ];
        result.c[ 0 ][ 1 ] =  axisY[ 0 ];
        result.c[ 0 ][ 2 ] = -axisZ[ 0 ];

        result.c[ 1 ][ 0 ] = -axisX[ 1 ];
        result.c[ 1 ][ 1 ] =  axisY[ 1 ];
        result.c[ 1 ][ 2 ] = -axisZ[ 1 ];

        result.c[ 2 ][ 0 ] = -axisX[ 2 ];
        result.c[ 2 ][ 1 ] =  axisY[ 2 ];
        result.c[ 2 ][ 2 ] = -axisZ[ 2 ];

        // translate (-posX, -posY, -posZ)
        result.c[ 3 ][ 0 ] =  axisX[ 0 ] * eyeX + axisX[ 1 ] * eyeY + axisX[ 2 ] * eyeZ;
        result.c[ 3 ][ 1 ] = -axisY[ 0 ] * eyeX - axisY[ 1 ] * eyeY - axisY[ 2 ] * eyeZ;
        result.c[ 3 ][ 2 ] =  axisZ[ 0 ] * eyeX + axisZ[ 1 ] * eyeY + axisZ[ 2 ] * eyeZ;
        result.c[ 3 ][ 3 ] = 1.0f;
    }

    static void LookAt(Matrix4f& result,
        const Vec3f& eye,
        const Vec3f& target,
        const Vec3f& up)
    {
        LookAt(result, eye[ 0 ], eye[ 1 ], eye[ 2 ], target[ 0 ], target[ 1 ], target[ 2 ], up[ 0 ], up[ 1 ], up[ 2 ]);
    }

    static Matrix4f LookAtMat(
        const Vec3f& eye,
        const Vec3f& target,
        const Vec3f& up)
    {
        Matrix4f m;
        Matrix4f::LookAt(m, eye, target, up);
        return m;
    }

    static Matrix4f PerspectiveMat(float fov, float aspect, float n, float f)
    {
        float frustumH = tanf(fov / 360.0f * Pi) * n;
        float frustumW = frustumH * aspect;
        return Matrix4f::PerspectiveMat(-frustumW, frustumW, -frustumH, frustumH, n, f);
    }

    static Matrix4f PerspectiveMat( float l, float r, float b, float t, float n, float f )
    {
        Matrix4f m;

        m.x[0] = 2 * n / (r - l);
        m.x[5] = 2 * n / (t - b);
        m.x[8] = (r + l) / (r - l);
        m.x[9] = (t + b) / (t - b);
        m.x[10] = -(f + n) / (f - n);
        m.x[11] = -1;
        m.x[14] = -2 * f * n / (f - n);
        m.x[15] = 0;

        return m;
    }

    static Matrix4f OrthoMat( float l, float r, float b, float t, float n, float f )
    {
        Matrix4f m;

        m.x[0] = 2 / (r - l);
        m.x[5] = 2 / (t - b);
        m.x[10] = -2 / (f - n);
        m.x[12] = -(r + l) / (r - l);
        m.x[13] = -(t + b) / (t - b);
        m.x[14] = -(f + n) / (f - n);

        return m;
    }

    static void fastMult43( Matrix4f &dst, const Matrix4f &m1, const Matrix4f &m2 )
    {
        // Note: dst may not be the same as m1 or m2

        float *dstx = dst.x;
        const float *m1x = m1.x;
        const float *m2x = m2.x;
        
        dstx[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2];
        dstx[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2];
        dstx[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2];
        dstx[3] = 0.0f;

        dstx[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6];
        dstx[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6];
        dstx[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6];
        dstx[7] = 0.0f;

        dstx[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10];
        dstx[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10];
        dstx[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10];
        dstx[11] = 0.0f;

        dstx[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
        dstx[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
        dstx[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
        dstx[15] = 1.0f;
    }

    // ------------
    // Constructors
    // ------------
    Matrix4f()
    {
        c[0][0] = 1; c[1][0] = 0; c[2][0] = 0; c[3][0] = 0;
        c[0][1] = 0; c[1][1] = 1; c[2][1] = 0; c[3][1] = 0;
        c[0][2] = 0; c[1][2] = 0; c[2][2] = 1; c[3][2] = 0;
        c[0][3] = 0; c[1][3] = 0; c[2][3] = 0; c[3][3] = 1;
    }

    explicit Matrix4f( NoInitHint )
    {
        // Constructor without default initialization
    }

    Matrix4f(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
             float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
    {
        x[ 0 ]  = m11; x[ 1 ]  = m21; x[ 2 ]  = m31; x[ 3 ]  = m41;
        x[ 4 ]  = m12; x[ 5 ]  = m22; x[ 6 ]  = m32; x[ 7 ]  = m42;
        x[ 8 ]  = m13; x[ 9 ]  = m23; x[ 10 ] = m33; x[ 11 ] = m43;
        x[ 12 ] = m14; x[ 13 ] = m24; x[ 14 ] = m34; x[ 15 ] = m44;
    }

    Matrix4f( const float *floatArray16 )
    {
        for(OFUInt32 i = 0; i < 4; ++i)
        {
            for(OFUInt32 j = 0; j < 4; ++j)
            {
                c[i][j] = floatArray16[i * 4 + j];
            }
        }
    }

    Matrix4f( const Quaternion &q )
    {
        // Calculate coefficients
        float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
        float xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
        float yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
        float wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;

        c[0][0] = 1 - (yy + zz);  c[1][0] = xy - wz;    
        c[2][0] = xz + wy;        c[3][0] = 0;
        c[0][1] = xy + wz;        c[1][1] = 1 - (xx + zz);
        c[2][1] = yz - wx;        c[3][1] = 0;
        c[0][2] = xz - wy;        c[1][2] = yz + wx;
        c[2][2] = 1 - (xx + yy);  c[3][2] = 0;
        c[0][3] = 0;              c[1][3] = 0;
        c[2][3] = 0;              c[3][3] = 1;
    }

    // ----------
    // Matrix sum
    // ----------
    Matrix4f operator+( const Matrix4f &m ) const 
    {
        Matrix4f mf( NO_INIT );
        
        mf.x[0]  = x[0]  + m.x[0];
        mf.x[1]  = x[1]  + m.x[1];
        mf.x[2]  = x[2]  + m.x[2];
        mf.x[3]  = x[3]  + m.x[3];
        mf.x[4]  = x[4]  + m.x[4];
        mf.x[5]  = x[5]  + m.x[5];
        mf.x[6]  = x[6]  + m.x[6];
        mf.x[7]  = x[7]  + m.x[7];
        mf.x[8]  = x[8]  + m.x[8];
        mf.x[9]  = x[9]  + m.x[9];
        mf.x[10] = x[10] + m.x[10];
        mf.x[11] = x[11] + m.x[11];
        mf.x[12] = x[12] + m.x[12];
        mf.x[13] = x[13] + m.x[13];
        mf.x[14] = x[14] + m.x[14];
        mf.x[15] = x[15] + m.x[15];

        return mf;
    }

    Matrix4f &operator+=( const Matrix4f &m )
    {
        return *this = *this + m;
    }

    Matrix4f &operator=(const Matrix4f &m)
    {
        memcpy(x, m.x, sizeof(Matrix4f));
        return *this;
    }

    // ---------------------
    // Matrix multiplication
    // ---------------------
#if defined(__ARM_NEON__) && defined(__aarch64__) && defined(OF_IOS)
    Matrix4f operator*( const Matrix4f &rhs ) const
    {
        static_assert(sizeof(float32x4x4_t) == sizeof(Matrix4f), "");
        float32x4x4_t iMatrixLeft = *(float32x4x4_t *)this;
        float32x4x4_t iMatrixRight = *(float32x4x4_t *)&rhs;
        float32x4x4_t m;
        
        m.val[0] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 0));
        m.val[1] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[1], 0));
        m.val[2] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[2], 0));
        m.val[3] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[3], 0));
        
        m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[0], 1));
        m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[1], 1));
        m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[2], 1));
        m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[3], 1));
        
        m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[0], 2));
        m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[1], 2));
        m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[2], 2));
        m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[3], 2));
        
        m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[0], 3));
        m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[1], 3));
        m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[2], 3));
        m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[3], 3));
        
        return *(Matrix4f *)&m;
    }
#else
    Matrix4f operator*( const Matrix4f &m ) const
    {
        Matrix4f mf( NO_INIT );
        
        mf.x[0] = x[0] * m.x[0] + x[4] * m.x[1] + x[8] * m.x[2] + x[12] * m.x[3];
        mf.x[1] = x[1] * m.x[0] + x[5] * m.x[1] + x[9] * m.x[2] + x[13] * m.x[3];
        mf.x[2] = x[2] * m.x[0] + x[6] * m.x[1] + x[10] * m.x[2] + x[14] * m.x[3];
        mf.x[3] = x[3] * m.x[0] + x[7] * m.x[1] + x[11] * m.x[2] + x[15] * m.x[3];

        mf.x[4] = x[0] * m.x[4] + x[4] * m.x[5] + x[8] * m.x[6] + x[12] * m.x[7];
        mf.x[5] = x[1] * m.x[4] + x[5] * m.x[5] + x[9] * m.x[6] + x[13] * m.x[7];
        mf.x[6] = x[2] * m.x[4] + x[6] * m.x[5] + x[10] * m.x[6] + x[14] * m.x[7];
        mf.x[7] = x[3] * m.x[4] + x[7] * m.x[5] + x[11] * m.x[6] + x[15] * m.x[7];

        mf.x[8] = x[0] * m.x[8] + x[4] * m.x[9] + x[8] * m.x[10] + x[12] * m.x[11];
        mf.x[9] = x[1] * m.x[8] + x[5] * m.x[9] + x[9] * m.x[10] + x[13] * m.x[11];
        mf.x[10] = x[2] * m.x[8] + x[6] * m.x[9] + x[10] * m.x[10] + x[14] * m.x[11];
        mf.x[11] = x[3] * m.x[8] + x[7] * m.x[9] + x[11] * m.x[10] + x[15] * m.x[11];

        mf.x[12] = x[0] * m.x[12] + x[4] * m.x[13] + x[8] * m.x[14] + x[12] * m.x[15];
        mf.x[13] = x[1] * m.x[12] + x[5] * m.x[13] + x[9] * m.x[14] + x[13] * m.x[15];
        mf.x[14] = x[2] * m.x[12] + x[6] * m.x[13] + x[10] * m.x[14] + x[14] * m.x[15];
        mf.x[15] = x[3] * m.x[12] + x[7] * m.x[13] + x[11] * m.x[14] + x[15] * m.x[15];

        return mf;
    }
#endif

    Matrix4f operator*( float f ) const
    {
        Matrix4f m( *this );
        
        m.x[0]  *= f; m.x[1]  *= f; m.x[2]  *= f; m.x[3]  *= f;
        m.x[4]  *= f; m.x[5]  *= f; m.x[6]  *= f; m.x[7]  *= f;
        m.x[8]  *= f; m.x[9]  *= f; m.x[10] *= f; m.x[11] *= f;
        m.x[12] *= f; m.x[13] *= f; m.x[14] *= f; m.x[15] *= f;

        return m;
    }

    // ----------------------------
    // Vector-Matrix multiplication
    // ----------------------------
#if defined(__ARM_NEON__) && defined(__aarch64__) && defined(OF_IOS)
    Vec3f operator*( const Vec3f &v ) const
    {
        Vec4f result = (*this) * Vec4f(v.x, v.y, v.z, 1.0f);
        return Vec3f(result.x, result.y, result.z);
    }
    
    Vec4f operator*( const Vec4f &vectorRight ) const
    {
        static_assert(sizeof(float32x4x4_t) == sizeof(Matrix4f), "");
        static_assert(sizeof(float32x4_t) == sizeof(Vec4f), "");
        
        float32x4x4_t iMatrix = *(float32x4x4_t *)this;
        float32x4_t v;
        
        iMatrix.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)vectorRight.x);
        iMatrix.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)vectorRight.y);
        iMatrix.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)vectorRight.z);
        iMatrix.val[3] = vmulq_n_f32(iMatrix.val[3], (float32_t)vectorRight.w);
        
        iMatrix.val[0] = vaddq_f32(iMatrix.val[0], iMatrix.val[1]);
        iMatrix.val[2] = vaddq_f32(iMatrix.val[2], iMatrix.val[3]);
        
        v = vaddq_f32(iMatrix.val[0], iMatrix.val[2]);
        
        return *(Vec4f *)&v;
    }
#else
    Vec3f operator*( const Vec3f &v ) const
    {
        return Vec3f(
            v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + c[3][0],
            v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + c[3][1],
            v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + c[3][2]
        );
    }

    Vec4f operator*( const Vec4f &v ) const
    {
        return Vec4f(
            v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + v.w * c[3][0],
            v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + v.w * c[3][1],
            v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + v.w * c[3][2],
            v.x * c[0][3] + v.y * c[1][3] + v.z * c[2][3] + v.w * c[3][3]
        );
    }
#endif

    Vec3f multiplyPoint3x4(const Vec3f &v) const
    {
        return (*this) * v;
    }

    Vec3f multiplyDirection(const Vec3f& v) const
    {
        float vx, vy, vz;

        vx = v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0];
        vy = v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1];
        vz = v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2];

        return Vec3f(vx, vy, vz);
    }

    void setIdentity()
    {
        memcpy(x, &IDENTITY, sizeof(float) * 16);
    }

    void setZero()
    {
        memset(x, 0, sizeof(float) * 16);
    }

    Vec3f mult33Vec( const Vec3f &v ) const
    {
        return Vec3f(
            v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + c[3][0],
            v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + c[3][1],
            v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + c[3][2]
        );
    }

    // ---------------
    // Transformations
    // ---------------
    void translate( const float x, const float y, const float z )
    {
        *this = TransMat( x, y, z ) * *this;
    }

    void scale( const float x, const float y, const float z )
    {
        *this = ScaleMat( x, y, z ) * *this;
    }

    void scaleMix( const float x, const float y, const float z )
    {
        c[0][0] *= x;  c[1][0] *= y;  c[2][0] *= z;
        c[0][1] *= x;  c[1][1] *= y;  c[2][1] *= z;
        c[0][2] *= x;  c[1][2] *= y;  c[2][2] *= z;
        c[0][3] *= x;  c[1][3] *= y;  c[2][3] *= z;
    }

    void rotate( const float x, const float y, const float z )
    {
        *this = RotMat( x, y, z ) * *this;
    }

    // ---------------
    // Other
    // ---------------
    Matrix4f& transpose()
    {
        for (OFUInt32 y = 0; y < 4; ++y)
        {
            for (OFUInt32 x = y + 1; x < 4; ++x)
            {
                float tmp = c[x][y];
                c[x][y] = c[y][x];
                c[y][x] = tmp;
            }
        }

        return *this;
    }

    Matrix4f transposed() const
    {
        Matrix4f m( *this );
        for(OFUInt32 y = 0; y < 4; ++y)
        {
            for(OFUInt32 x = y + 1; x < 4; ++x) 
            {
                float tmp = m.c[x][y];
                m.c[x][y] = m.c[y][x];
                m.c[y][x] = tmp;
            }
        }

        return m;
    }

    float determinant() const
    {
        return 
            c[0][3]*c[1][2]*c[2][1]*c[3][0] - c[0][2]*c[1][3]*c[2][1]*c[3][0] - c[0][3]*c[1][1]*c[2][2]*c[3][0] + c[0][1]*c[1][3]*c[2][2]*c[3][0] +
            c[0][2]*c[1][1]*c[2][3]*c[3][0] - c[0][1]*c[1][2]*c[2][3]*c[3][0] - c[0][3]*c[1][2]*c[2][0]*c[3][1] + c[0][2]*c[1][3]*c[2][0]*c[3][1] +
            c[0][3]*c[1][0]*c[2][2]*c[3][1] - c[0][0]*c[1][3]*c[2][2]*c[3][1] - c[0][2]*c[1][0]*c[2][3]*c[3][1] + c[0][0]*c[1][2]*c[2][3]*c[3][1] +
            c[0][3]*c[1][1]*c[2][0]*c[3][2] - c[0][1]*c[1][3]*c[2][0]*c[3][2] - c[0][3]*c[1][0]*c[2][1]*c[3][2] + c[0][0]*c[1][3]*c[2][1]*c[3][2] +
            c[0][1]*c[1][0]*c[2][3]*c[3][2] - c[0][0]*c[1][1]*c[2][3]*c[3][2] - c[0][2]*c[1][1]*c[2][0]*c[3][3] + c[0][1]*c[1][2]*c[2][0]*c[3][3] +
            c[0][2]*c[1][0]*c[2][1]*c[3][3] - c[0][0]*c[1][2]*c[2][1]*c[3][3] - c[0][1]*c[1][0]*c[2][2]*c[3][3] + c[0][0]*c[1][1]*c[2][2]*c[3][3];
    }

    Matrix4f inverted() const
    {
        Matrix4f m( NO_INIT );

        float d = determinant();
        if( d == 0 ) return m;
        d = 1.0f / d;

        m.c[0][0] = d * (c[1][2]*c[2][3]*c[3][1] - c[1][3]*c[2][2]*c[3][1] + c[1][3]*c[2][1]*c[3][2] - c[1][1]*c[2][3]*c[3][2] - c[1][2]*c[2][1]*c[3][3] + c[1][1]*c[2][2]*c[3][3]);
        m.c[0][1] = d * (c[0][3]*c[2][2]*c[3][1] - c[0][2]*c[2][3]*c[3][1] - c[0][3]*c[2][1]*c[3][2] + c[0][1]*c[2][3]*c[3][2] + c[0][2]*c[2][1]*c[3][3] - c[0][1]*c[2][2]*c[3][3]);
        m.c[0][2] = d * (c[0][2]*c[1][3]*c[3][1] - c[0][3]*c[1][2]*c[3][1] + c[0][3]*c[1][1]*c[3][2] - c[0][1]*c[1][3]*c[3][2] - c[0][2]*c[1][1]*c[3][3] + c[0][1]*c[1][2]*c[3][3]);
        m.c[0][3] = d * (c[0][3]*c[1][2]*c[2][1] - c[0][2]*c[1][3]*c[2][1] - c[0][3]*c[1][1]*c[2][2] + c[0][1]*c[1][3]*c[2][2] + c[0][2]*c[1][1]*c[2][3] - c[0][1]*c[1][2]*c[2][3]);
        m.c[1][0] = d * (c[1][3]*c[2][2]*c[3][0] - c[1][2]*c[2][3]*c[3][0] - c[1][3]*c[2][0]*c[3][2] + c[1][0]*c[2][3]*c[3][2] + c[1][2]*c[2][0]*c[3][3] - c[1][0]*c[2][2]*c[3][3]);
        m.c[1][1] = d * (c[0][2]*c[2][3]*c[3][0] - c[0][3]*c[2][2]*c[3][0] + c[0][3]*c[2][0]*c[3][2] - c[0][0]*c[2][3]*c[3][2] - c[0][2]*c[2][0]*c[3][3] + c[0][0]*c[2][2]*c[3][3]);
        m.c[1][2] = d * (c[0][3]*c[1][2]*c[3][0] - c[0][2]*c[1][3]*c[3][0] - c[0][3]*c[1][0]*c[3][2] + c[0][0]*c[1][3]*c[3][2] + c[0][2]*c[1][0]*c[3][3] - c[0][0]*c[1][2]*c[3][3]);
        m.c[1][3] = d * (c[0][2]*c[1][3]*c[2][0] - c[0][3]*c[1][2]*c[2][0] + c[0][3]*c[1][0]*c[2][2] - c[0][0]*c[1][3]*c[2][2] - c[0][2]*c[1][0]*c[2][3] + c[0][0]*c[1][2]*c[2][3]);
        m.c[2][0] = d * (c[1][1]*c[2][3]*c[3][0] - c[1][3]*c[2][1]*c[3][0] + c[1][3]*c[2][0]*c[3][1] - c[1][0]*c[2][3]*c[3][1] - c[1][1]*c[2][0]*c[3][3] + c[1][0]*c[2][1]*c[3][3]);
        m.c[2][1] = d * (c[0][3]*c[2][1]*c[3][0] - c[0][1]*c[2][3]*c[3][0] - c[0][3]*c[2][0]*c[3][1] + c[0][0]*c[2][3]*c[3][1] + c[0][1]*c[2][0]*c[3][3] - c[0][0]*c[2][1]*c[3][3]);
        m.c[2][2] = d * (c[0][1]*c[1][3]*c[3][0] - c[0][3]*c[1][1]*c[3][0] + c[0][3]*c[1][0]*c[3][1] - c[0][0]*c[1][3]*c[3][1] - c[0][1]*c[1][0]*c[3][3] + c[0][0]*c[1][1]*c[3][3]);
        m.c[2][3] = d * (c[0][3]*c[1][1]*c[2][0] - c[0][1]*c[1][3]*c[2][0] - c[0][3]*c[1][0]*c[2][1] + c[0][0]*c[1][3]*c[2][1] + c[0][1]*c[1][0]*c[2][3] - c[0][0]*c[1][1]*c[2][3]);
        m.c[3][0] = d * (c[1][2]*c[2][1]*c[3][0] - c[1][1]*c[2][2]*c[3][0] - c[1][2]*c[2][0]*c[3][1] + c[1][0]*c[2][2]*c[3][1] + c[1][1]*c[2][0]*c[3][2] - c[1][0]*c[2][1]*c[3][2]);
        m.c[3][1] = d * (c[0][1]*c[2][2]*c[3][0] - c[0][2]*c[2][1]*c[3][0] + c[0][2]*c[2][0]*c[3][1] - c[0][0]*c[2][2]*c[3][1] - c[0][1]*c[2][0]*c[3][2] + c[0][0]*c[2][1]*c[3][2]);
        m.c[3][2] = d * (c[0][2]*c[1][1]*c[3][0] - c[0][1]*c[1][2]*c[3][0] - c[0][2]*c[1][0]*c[3][1] + c[0][0]*c[1][2]*c[3][1] + c[0][1]*c[1][0]*c[3][2] - c[0][0]*c[1][1]*c[3][2]);
        m.c[3][3] = d * (c[0][1]*c[1][2]*c[2][0] - c[0][2]*c[1][1]*c[2][0] + c[0][2]*c[1][0]*c[2][1] - c[0][0]*c[1][2]*c[2][1] - c[0][1]*c[1][0]*c[2][2] + c[0][0]*c[1][1]*c[2][2]);

        return m;
    }

    void decompose( Vec3f &trans, Vec3f &rot, Vec3f &scale ) const
    {
        // Getting translation is trivial
        trans = Vec3f( c[3][0], c[3][1], c[3][2] );

        // Scale is length of columns
        scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
        scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
        scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );

        if( scale.x == 0 || scale.y == 0 || scale.z == 0 ) return;

        // Detect negative scale with determinant and flip one arbitrary axis
        if( determinant() < 0 ) scale.x = -scale.x;

        // Combined rotation matrix YXZ
        //
        // Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]   Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]  Cos[x]*Sin[y]    
        // Cos[x]*Sin[z]                        Cos[x]*Cos[z]                       -Sin[x]
        // -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]  Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]  Cos[x]*Cos[y]

        rot.x = asinf( -c[2][1] / scale.z );
        
        // Special case: Cos[x] == 0 (when Sin[x] is +/-1)
        float f = fabsf( c[2][1] / scale.z );
        if( f > 0.999f && f < 1.001f )
        {
            // Pin arbitrarily one of y or z to zero
            // Mathematical equivalent of gimbal lock
            rot.y = 0;
            
            // Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
            // => m[0][0] = Cos[z] and m[1][0] = Sin[z]
            rot.z = atan2f( -c[1][0] / scale.y, c[0][0] / scale.x );
        }
        // Standard case
        else
        {
            rot.y = atan2f( c[2][0] / scale.z, c[2][2] / scale.z );
            rot.z = atan2f( c[0][1] / scale.x, c[1][1] / scale.y );
        }
    }

    void extractMatrix3x3f(float matrix33[ 3 ][ 3 ])
    {
        for (OFInt32 i = 0; i < 3; ++i)
        {
            for (OFInt32 j = 0; j < 3; ++j)
            {
                matrix33[ i ][ j ] = c[ i ][ j ];
            }
        }
    }

    void set(const float *floatArray16)
    {
        for (OFUInt32 i = 0; i < 4; ++i)
        {
            for (OFUInt32 j = 0; j < 4; ++j)
            {
                c[i][j] = floatArray16[i * 4 + j];
            }
        }
    }

    void setCol( OFUInt32 col, const Vec4f& v ) 
    {
        x[col * 4 + 0] = v.x;
        x[col * 4 + 1] = v.y;
        x[col * 4 + 2] = v.z;
        x[col * 4 + 3] = v.w;
    }

    Vec4f getCol( OFUInt32 col ) const
    {
        return Vec4f( x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3] );
    }

    Vec4f getRow( OFUInt32 row ) const
    {
        return Vec4f( x[row + 0], x[row + 4], x[row + 8], x[row + 12] );
    }

    Vec3f getTrans() const
    {
        return Vec3f( c[3][0], c[3][1], c[3][2] );
    }
    
    // Note: Scale length is length of columns
    Vec3f getScaleLen() const
    {
        Vec3f scale;
        scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
        scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
        scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );
        return scale;
    }

    Vec3f getScale() const
    {
        return Vec3f(c[0][0], c[1][1], c[2][2]);
    }

    Vec3f getRot() const
    {
        Vec3f t, r, s;
        decompose(t, r, s);
        return r;
    }

    Quaternion getQuat() const
    {
#if 0
        Quaternion q;
        q.w = sqrtf(fmaxf(0, 1 + c[0][0] + c[1][1] + c[2][2])) / 2;
        q.x = sqrtf(fmaxf(0, 1 + c[0][0] - c[1][1] - c[2][2])) / 2;
        q.y = sqrtf(fmaxf(0, 1 - c[0][0] + c[1][1] - c[2][2])) / 2;
        q.z = sqrtf(fmaxf(0, 1 - c[0][0] - c[1][1] + c[2][2])) / 2;
        q.x *= Sign(q.x * (c[2][1] - c[1][2]));
        q.y *= Sign(q.y * (c[0][2] - c[2][0]));
        q.z *= Sign(q.z * (c[1][0] - c[0][1]));
//#else
        // refs: http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche52.html
        q.x = (c[0][0] + c[1][1] + c[2][2] + 1.0f) / 4.0f;
        q.y = (c[0][0] - c[1][1] - c[2][2] + 1.0f) / 4.0f;
        q.z = (-c[0][0] + c[1][1] - c[2][2] + 1.0f) / 4.0f;
        q.w = (-c[0][0] - c[1][1] + c[2][2] + 1.0f) / 4.0f;
        if (q.x < 0.0f) q.x = 0.0f;
        if (q.y < 0.0f) q.y = 0.0f;
        if (q.z < 0.0f) q.z = 0.0f;
        if (q.w < 0.0f) q.w = 0.0f;
        q.x = sqrt(q.x);
        q.y = sqrt(q.y);
        q.z = sqrt(q.z);
        q.w = sqrt(q.w);
        if (q.x >= q.y && q.x >= q.z && q.x >= q.w)
        {
            q.x *= +1.0f;
            q.y *= Sign(c[2][1] - c[1][2]);
            q.z *= Sign(c[0][2] - c[2][0]);
            q.w *= Sign(c[1][0] - c[0][1]);
        }
        else if (q.y >= q.x && q.y >= q.z && q.y >= q.w)
        {
            q.x *= Sign(c[2][1] - c[1][2]);
            q.y *= +1.0f;
            q.z *= Sign(c[1][0] + c[0][1]);
            q.w *= Sign(c[0][2] + c[2][0]);
        }
        else if (q.z >= q.x && q.z >= q.y && q.z >= q.w)
        {
            q.x *= Sign(c[0][2] - c[2][0]);
            q.y *= Sign(c[1][0] + c[0][2]);
            q.z *= +1.0f;
            q.w *= Sign(c[2][1] + c[1][2]);
        }
        else if (q.w >= q.x && q.w >= q.y && q.w >= q.z)
        {
            q.x *= Sign(c[1][0] - c[0][1]);
            q.y *= Sign(c[2][0] + c[0][2]);
            q.z *= Sign(c[2][1] + c[1][2]);
            q.w *= +1.0f;
        }
        else
        {
            printf("Matrix.getQuat error\n");
        }

        float r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
        q.x /= r;
        q.y /= r;
        q.z /= r;
        q.w /= r;
#elseif 0
        Quaternion q;
        // refs: u3d Quaternion::MatrixToQuaternion.
        // the x angle is wrong!
        float fTrace = c[0][0] + c[1][1] + c[2][2];
        float fRoot;

        if (fTrace > 0.0f)
        {
            // |w| > 1/2, may as well choose w > 1/2
            fRoot = std::sqrt(fTrace + 1.0f);   // 2w
            q.w = 0.5f * fRoot;
            fRoot = 0.5f / fRoot;  // 1/(4w)
            q.x = (c[2][1] - c[1][2]) * fRoot;
            q.y = (c[0][2] - c[2][0]) * fRoot;
            q.z = (c[1][0] - c[0][1]) * fRoot;
        }
        else
        {
            // |w| <= 1/2
            int s_iNext[3] = { 1, 2, 0 };
            int i = 0;
            if (c[1][1] > c[0][0])
            {
                i = 1;
            }
            if (c[2][2] > c[i][i])
            {
                i = 2;
            }
            int j = s_iNext[i];
            int k = s_iNext[j];

            fRoot = std::sqrt(c[i][i] - c[j][j] - c[k][k] + 1.0f);
            float* apkQuat[3] = { &q.x, &q.y, &q.z };
            assert(fRoot >= 0.0001f);
            *apkQuat[i] = 0.5f * fRoot;
            fRoot = 0.5f / fRoot;
            q.w = (c[k][j] - c[j][k]) * fRoot;
            *apkQuat[j] = (c[j][i] + c[i][j]) * fRoot;
            *apkQuat[k] = (c[k][i] + c[i][k]) * fRoot;
        }
        float r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
        q.x /= r;
        q.y /= r;
        q.z /= r;
        q.w /= r;
#else
        Vec3f t, r, s;
        decompose(t, r, s);
        Quaternion q(r.x, r.y, r.z);
#endif
        return q;
    }
};


// -------------------------------------------------------------------------------------------------
// Plane
// -------------------------------------------------------------------------------------------------

class OF_CORE_API Plane
{
public:
    Vec3f normal; 
    float dist;

    // ------------
    // Constructors
    // ------------
    Plane() 
    { 
        normal.x = 0; normal.y = 0; normal.z = 0; dist = 0; 
    };

    explicit Plane( const float a, const float b, const float c, const float d )
    {
        normal = Vec3f( a, b, c );
        float invLen = 1.0f / normal.length();
        normal *= invLen;    // Normalize
        dist = d * invLen;
    }

    Plane( const Vec3f &v0, const Vec3f &v1, const Vec3f &v2 )
    {
        normal = v1 - v0;
        normal = normal.cross( v2 - v0 );
        normal.normalize();
        dist = -normal.dot( v0 );
    }

    // ----------------
    // Other operations
    // ----------------
    float distToPoint( const Vec3f &v ) const
    {
        return normal.dot( v ) + dist;
    }
};


// -------------------------------------------------------------------------------------------------
// Bounding Box
// -------------------------------------------------------------------------------------------------
struct OF_CORE_API BoundingBox
{
    Vec3f min, max;

    BoundingBox()
    {
        min.set(MaxFloat, MaxFloat, MaxFloat);
        max.set(-MaxFloat, -MaxFloat, -MaxFloat);
    }

    BoundingBox(const Vec3f& _min, const Vec3f& _max)
    {
        min = _min;
        max = _max;
    }

    void clear()
    {
        min = Vec3f(0.0f, 0.0f, 0.0f);
        max = Vec3f(0.0f, 0.0f, 0.0f);
    }

    /*
             7 +----------+ 6
              /          /|
             /          / |
          3 +----------+  | 
            |  +     2 |  + 5
            |  4       | /
            |          |/
          0 +----------+ 1
    */
    Vec3f getCorner(OFUInt32 index) const
    {
        switch (index)
        {
        case 0:
            return Vec3f(min.x, min.y, max.z);
        case 1:
            return Vec3f(max.x, min.y, max.z);
        case 2:
            return Vec3f(max.x, max.y, max.z);
        case 3:
            return Vec3f(min.x, max.y, max.z);
        case 4:
            return Vec3f(min.x, min.y, min.z);
        case 5:
            return Vec3f(max.x, min.y, min.z);
        case 6:
            return Vec3f(max.x, max.y, min.z);
        case 7:
            return Vec3f(min.x, max.y, min.z);
        default:
            return Vec3f();
        }
    }

    void transform(const Matrix4f &m)
    {
        //
        // Efficient algorithm for transforming an AABB, taken from Graphics Gems
        //
        float minA[ 3 ] = { min.x, min.y, min.z }, minB[ 3 ];
        float maxA[ 3 ] = { max.x, max.y, max.z }, maxB[ 3 ];

        for (OFUInt32 i = 0; i < 3; ++i)
        {
            minB[ i ] = m.c[ 3 ][ i ];
            maxB[ i ] = m.c[ 3 ][ i ];

            for (OFUInt32 j = 0; j < 3; ++j)
            {
                float x = minA[ j ] * m.c[ j ][ i ];
                float y = maxA[ j ] * m.c[ j ][ i ];
                minB[ i ] += Min(x, y);
                maxB[ i ] += Max(x, y);
            }
        }

        min = Vec3f(minB[ 0 ], minB[ 1 ], minB[ 2 ]);
        max = Vec3f(maxB[ 0 ], maxB[ 1 ], maxB[ 2 ]);
    }

    bool makeUnion(const BoundingBox &b)
    {
        bool changed = false;
        //
        // Ignore zero-size boxes
        //
        if (min == max)
        {
            changed = true;
            min = b.min;
            max = b.max;
        }
        else if (b.min != b.max)
        {
            if (b.min.x < min.x)
            {
                changed = true; min.x = b.min.x;
            }
            if (b.min.y < min.y)
            {
                changed = true; min.y = b.min.y;
            }
            if (b.min.z < min.z)
            {
                changed = true; min.z = b.min.z;
            }

            if (b.max.x > max.x)
            {
                changed = true; max.x = b.max.x;
            }
            if (b.max.y > max.y)
            {
                changed = true; max.y = b.max.y;
            }
            if (b.max.z > max.z)
            {
                changed = true; max.z = b.max.z;
            }
        }

        return changed;
    }

    void updateMinMax(const Vec3f* point, OFUInt64 num)
    {
        for (OFUInt64 i = 0; i < num; i++)
        {
            // Leftmost point.
            if (point[i].x < min.x)
            {
                min.x = point[i].x;
            }

            // Lowest point.
            if (point[i].y < min.y)
            {
                min.y = point[i].y;
            }

            // Farthest point.
            if (point[i].z < min.z)
            {
                min.z = point[i].z;
            }

            // Rightmost point.
            if (point[i].x > max.x)
            {
                max.x = point[i].x;
            }

            // Highest point.
            if (point[i].y > max.y)
            {
                max.y = point[i].y;
            }

            // Nearest point.
            if (point[i].z > max.z)
            {
                max.z = point[i].z;
            }
        }
    }
};

struct AABB2Df
{
    Vec2f mini, maxi;
    AABB2Df()
    {
        mini = Vec2f(1e7, 1e7);
        maxi = Vec2f(-1e7, -1e7);
    }
    inline void addVertex(const Vec2f v)
    {
        if (mini.x > v.x)
        {
            mini.x = v.x;
        }
        if (mini.y > v.y)
        {
            mini.y = v.y;
        }
        if (maxi.x < v.x)
        {
            maxi.x = v.x;
        }
        if (maxi.y < v.y)
        {
            maxi.y = v.y;
        }
    }
};


// -------------------------------------------------------------------------------------------------
// Frustum
// -------------------------------------------------------------------------------------------------
class OF_CORE_API Frustum
{
public:
    const Vec3f &getOrigin() const
    {
        return _origin;
    }

    const Vec3f &getCorner(OFUInt32 index) const
    {
        return _corners[ index ];
    }

    void buildViewFrustum(const Matrix4f &transMat, float fov, float aspect, float nearPlane, float farPlane);
    void buildViewFrustum(
        const Matrix4f &transMat,
        float left, float right, float bottom, float top,
        float nearPlane, float farPlane
    );
    void buildViewFrustum(const Matrix4f &viewMat, const Matrix4f &projMat);
    void buildBoxFrustum(
        const Matrix4f &transMat,
        float left, float right, float bottom, float top, float front, float back
    );
    bool cullSphere(Vec3f pos, float rad) const;
    bool cullBox(BoundingBox &b) const;
    bool cullFrustum(const Frustum &frust) const;
    void calcAABB(Vec3f &mins, Vec3f &maxs) const;

private:
    Plane _planes[ 6 ];  // Planes of frustum
    Vec3f _origin;
    Vec3f _corners[ 8 ];  // Corner points
};

// -------------------------------------------------------------------------------------------------
// Intersection
// -------------------------------------------------------------------------------------------------

inline bool rayTriangleIntersection(
    const Vec3f &rayOrig, const Vec3f &rayDir,
    const Vec3f &vert0, const Vec3f &vert1, const Vec3f &vert2,
    Vec3f &intsPoint)
{
    // Idea: Tomas Moeller and Ben Trumbore
    // in Fast, Minimum Storage Ray/Triangle Intersection 
    
    // Find vectors for two edges sharing vert0
    Vec3f edge1 = vert1 - vert0;
    Vec3f edge2 = vert2 - vert0;

    // Begin calculating determinant - also used to calculate U parameter
    Vec3f pvec = rayDir.cross( edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    float det = edge1.dot( pvec );

    // *** Culling branch ***
    /*if( det < Epsilon )return false;

    // Calculate distance from vert0 to ray origin
    Vec3f tvec = rayOrig - vert0;

    // Calculate U parameter and test bounds
    float u = tvec.dot( pvec );
    if (u < 0 || u > det ) return false;

    // Prepare to test V parameter
    Vec3f qvec = tvec.cross( edge1 );

    // Calculate V parameter and test bounds
    float v = rayDir.dot( qvec );
    if (v < 0 || u + v > det ) return false;

    // Calculate t, scale parameters, ray intersects triangle
    float t = edge2.dot( qvec ) / det;*/


    // *** Non-culling branch ***
    if( det > -Epsilon && det < Epsilon ) return 0;
    float inv_det = 1.0f / det;

    // Calculate distance from vert0 to ray origin
    Vec3f tvec = rayOrig - vert0;

    // Calculate U parameter and test bounds
    float u = tvec.dot( pvec ) * inv_det;
    if( u < 0.0f || u > 1.0f ) return 0;

    // Prepare to test V parameter
    Vec3f qvec = tvec.cross( edge1 );

    // Calculate V parameter and test bounds
    float v = rayDir.dot( qvec ) * inv_det;
    if( v < 0.0f || u + v > 1.0f ) return 0;

    // Calculate t, ray intersects triangle
    float t = edge2.dot( qvec ) * inv_det;

    // Calculate intersection point and test ray length and direction
    intsPoint = rayOrig + rayDir * t;
    Vec3f vec = intsPoint - rayOrig;
    if (vec.dot(rayDir) < 0 || vec.length() > rayDir.length())
    {
        return false;
    }

    return true;
}

inline bool rayAABBIntersection(
    const Vec3f &rayOrig, const Vec3f &rayDir,
    const Vec3f &mins, const Vec3f &maxs)
{
    // SLAB based optimized ray/AABB intersection routine
    // Idea taken from http://ompf.org/ray/

    float l1 = (mins.x - rayOrig.x) / rayDir.x;
    float l2 = (maxs.x - rayOrig.x) / rayDir.x;
    float lmin = Min( l1, l2 );
    float lmax = Max( l1, l2 );

    l1 = (mins.y - rayOrig.y) / rayDir.y;
    l2 = (maxs.y - rayOrig.y) / rayDir.y;
    lmin = Max( Min( l1, l2 ), lmin );
    lmax = Min( Max( l1, l2 ), lmax );

    l1 = (mins.z - rayOrig.z) / rayDir.z;
    l2 = (maxs.z - rayOrig.z) / rayDir.z;
    lmin = Max( Min( l1, l2 ), lmin );
    lmax = Min( Max( l1, l2 ), lmax );

    if( (lmax >= 0.0f) & (lmax >= lmin) )
    {
        // Consider length
        const Vec3f rayDest = rayOrig + rayDir;
        Vec3f rayMins( Min( rayDest.x, rayOrig.x), Min( rayDest.y, rayOrig.y ), Min( rayDest.z, rayOrig.z ) );
        Vec3f rayMaxs( Max( rayDest.x, rayOrig.x), Max( rayDest.y, rayOrig.y ), Max( rayDest.z, rayOrig.z ) );
        return 
            (rayMins.x < maxs.x) && (rayMaxs.x > mins.x) &&
            (rayMins.y < maxs.y) && (rayMaxs.y > mins.y) &&
            (rayMins.z < maxs.z) && (rayMaxs.z > mins.z);
    }
    else
    {
        return false;
    }
}

inline float nearestDistToAABB( const Vec3f &pos, const Vec3f &mins, const Vec3f &maxs )
{
    const Vec3f center = (mins + maxs) * 0.5f;
    const Vec3f extent = (maxs - mins) * 0.5f;

    Vec3f nearestVec;
    nearestVec.x = Max( 0.0f, fabsf( pos.x - center.x ) - extent.x );
    nearestVec.y = Max( 0.0f, fabsf( pos.y - center.y ) - extent.y );
    nearestVec.z = Max( 0.0f, fabsf( pos.z - center.z ) - extent.z );

    return nearestVec.length();
}

inline float Distance(float* p1, float* p2)
{
    float a = p1[ 0 ] - p2[ 0 ];
    float b = p1[ 1 ] - p2[ 1 ];
    return sqrtf(a * a + b * b);
}

inline float Distance(const Vec2f& p1, const Vec2f& p2)
{
    return (p1 - p2).length();
}

OF_CORE_API bool GetAffineTransform(const Vec2f src[], const Vec2f dst[], OFUInt32 nPt, double* affineMatrix);

OF_CORE_API bool GetRotationMatrix2D(double cx, double cy, double angle, double scaleX, double scaleY, double* affineMatrix);

OF_CORE_API bool MultiplyMatrix2D(const double* a, const double* b, double* c);

OF_CORE_API bool IsTwoRectOverlap(OFInt32 ax, OFInt32 ay, OFInt32 aw, OFInt32 ah, OFInt32 bx, OFInt32 by, OFInt32 bw, OFInt32 bh);

}
