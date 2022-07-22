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
