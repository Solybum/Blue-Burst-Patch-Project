#pragma once

#include <cmath>
#include <cstdint>
#include <windows.h>

#include "../map_object.h"
#include "../mathutil.h"

#pragma pack(push, 0)
struct Vertex
{
    float x;
    float y;
    float z;
    float u;
    float v;
};
#pragma pack(pop)

// A trail that drags behind the wisp
// Implemented as a ribbon mesh with IK movement
class RibbonTrail
{
private:
    // Parameters
    const size_t numSegments;
    const float segmentLength;
    const float ribbonWidth;
    const float jointStiffness;

    // State
    std::vector<Vec3f> simulationPoints;
    std::vector<Vertex> meshVertices;
    std::vector<uint16_t> meshIndices;

public:
    RibbonTrail(
        size_t numSegments,
        float segmentLength,
        float ribbonWidth,
        float jointStiffness);
    void Update(Vec3f headPos);
    void Render() const;
};

class MapObjectSnowWisp : public MapObject::MapObjectBase
{
public:
    static void __cdecl LoadAssets();
    static void __cdecl UnloadAssets();
    static void* __cdecl Create(MapObject::InitData::InnerData* initData);

    MapObjectSnowWisp(void* parentObject, MapObject::InitData::InnerData* initData);

    void Destruct(BOOL freeMemory);
    void Update();
    void Render();

private:
    // Parameters
    const float forwardSpeed;
    const float turnSpeed;
    const size_t turnMaxAngleDeg;
    const size_t turnFrameIntervalRandMin;
    const size_t turnFrameIntervalRandMax;
    const float swayAmplitudeHorizontal;
    const float swayAmplitudeVertical;
    const float swayFrequencyHorizontal;
    const float swayFrequencyVertical;

    // State
    size_t frameCounter;
    int framesUntilNextTurn;
    Vec3f forwardDirection;
    Vec3f targetDirection;
    float turnProgress;

    Vec3f lastGroundPosition;
    RibbonTrail ribbonTrail;
    char rngState[0xe8];

    void SnapToMapSurface();
    float RandomRange(float min, float max);
    void ChangeDirection(Vec3f newDir);
};
