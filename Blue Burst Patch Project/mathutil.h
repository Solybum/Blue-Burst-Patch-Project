#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdint>

#pragma pack(push, 1)
/// Vector of three components
template<typename T>
struct Vec3
{
    T x;
    T y;
    T z;

    // Note that there are no implicit constructors for volatiles.
    // We also can't add constructors or copy-assignment operators because they are
    // not allowed in unnamed struct fields which is what we use for DEFINE_FIELD.

    volatile Vec3& operator+(volatile Vec3 const& other) volatile
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    void set(volatile Vec3 const& other) volatile
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    void set(volatile Vec3 const* other) volatile
    {
        x = other->x;
        y = other->y;
        z = other->z;
    }
};
#pragma pack(pop)

typedef Vec3<float> Vec3f;

template<typename T>
auto DistanceSquaredXZ(Vec3<T> const& a, Vec3<T> const& b)
{
    auto dx = a.x - b.x;
    auto dz = a.z - b.z;
    return fabs(dx * dx + dz * dz);
}

template<typename T>
auto DistanceSquaredXZ(volatile Vec3<T> const& a, volatile Vec3<T> const& b)
{
    auto dx = a.x - b.x;
    auto dz = a.z - b.z;
    return fabs(dx * dx + dz * dz);
}

template <typename T>
constexpr T Squared(const T x) { return x * x; }

double RadDistance(double a, double b);

double RadLerp(double a, double b, double t);

/// Assuming input is in range 0..0xffff
double IntAngleToRad(int16_t a);

/// Assuming input is in range -pi..pi
int16_t RadToIntAngle(double a);

template<typename T>
int Sign(T val) {
    return (T(0) < val) - (val < T(0));
}

template<typename T>
T Sign(Vec3<T> p1, Vec3<T> p2, Vec3<T> p3)
{
    return (p1.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (p1.z - p3.z);
}

template<typename T>
bool PointInTriangle (Vec3<T> pt, Vec3<T> v1, Vec3<T> v2, Vec3<T> v3)
{
    T d1, d2, d3;
    bool has_neg, has_pos;

    d1 = Sign(pt, v1, v2);
    d2 = Sign(pt, v2, v3);
    d3 = Sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

double DegToRad(double deg);
double RadToDeg(double rad);
