#pragma once

#include <string>
#include <vector>
#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>
#include <assimp/quaternion.h>
#include <assimp/anim.h>

struct KeyPosition
{
    aiVector3D position;
    float timeStamp;
};

struct KeyRotation
{
    aiQuaternion orientation;
    float timeStamp;
};

struct KeyScale
{
    aiVector3D scale;
    float timeStamp;
};

class Bone
{
private:
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
    aiMatrix4x4 localTransform; // Default constructor sets values to identity
    std::string name;
    size_t id;

public:
    Bone(const std::string& name, size_t id, const aiNodeAnim* channel);
    void Update(float animationTime);
    const std::string& GetName() const;
    aiMatrix4x4 GetLocalTransform() const;

private:
    aiVector3D InterpolatePosition(float animationTime);
    aiQuaternion InterpolateRotation(float animationTime);
    aiVector3D InterpolateScaling(float animationTime);
};
