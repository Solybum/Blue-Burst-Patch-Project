#define _USE_MATH_DEFINES

#include <cstdint>
#include <cmath>

#include "mathutil.h"

double RadDistance(double a, double b)
{
    auto d = fmod(b - a, M_PI * 2.0);
    return fmod(d * 2.0, M_PI * 2.0) - d;
}

double RadLerp(double a, double b, double t)
{
    return a + RadDistance(a, b) * t;
}

double IntAngleToRad(uint32_t a)
{
    return a / 65535.0 * M_PI * 2.0 - M_PI;
}

uint32_t RadToIntAngle(double a)
{
    return uint32_t(a * 65535.0 / (M_PI * 2.0));
}

double DegToRad(double deg)
{
    return deg * (M_PI / 180.0);
}

double RadToDeg(double rad)
{
    return rad * (180.0 / M_PI);
}

#undef _USE_MATH_DEFINES
