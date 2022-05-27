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

    Vec3() : x(), y(), z() {}
    Vec3(Vec3 const& other) : x(other.x), y(other.y), z(other.z) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    Vec3(volatile Vec3 const& other) : x(other.x), y(other.y), z(other.z) {}

    /// Copying a volatile
    volatile Vec3& operator=(volatile Vec3 const& other) volatile
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    volatile Vec3& operator+(volatile Vec3 const& other) volatile
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
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
