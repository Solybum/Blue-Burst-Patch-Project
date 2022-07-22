#ifdef USE_NEWGFX

#include <stdexcept>
#include "bone.h"

Bone::Bone(const std::string& name, size_t id, const aiNodeAnim* channel) :
    name(name), id(id)
{
    // Extract the position, rotation and scaling keyframes
    for (auto i = 0; i < channel->mNumPositionKeys; i++)
    {
        KeyPosition keyframe;
        keyframe.position = channel->mPositionKeys[i].mValue;
        keyframe.timeStamp = channel->mPositionKeys[i].mTime;
        positions.push_back(keyframe);
    }

    for (auto i = 0; i < channel->mNumRotationKeys; i++)
    {
        KeyRotation keyframe;
        keyframe.orientation = channel->mRotationKeys[i].mValue;
        keyframe.timeStamp = channel->mRotationKeys[i].mTime;
        rotations.push_back(keyframe);
    }

    for (auto i = 0; i < channel->mNumScalingKeys; i++)
    {
        KeyScale keyframe;
        keyframe.scale = channel->mScalingKeys[i].mValue;
        keyframe.timeStamp = channel->mScalingKeys[i].mTime;
        scales.push_back(keyframe);
    }
}

const std::string& Bone::GetName() const
{
    return name;
}

aiMatrix4x4 Bone::GetLocalTransform() const
{
    return localTransform;
}

/// Apply the transformations at the specified timestamp
void Bone::Update(float animationTime)
{
    auto translation = InterpolatePosition(animationTime);
    auto rotation = InterpolateRotation(animationTime);
    auto scale = InterpolateScaling(animationTime);
    localTransform = aiMatrix4x4(scale, rotation, translation);
}

/// Find the keyframe whose timestamp is nearest to the current time
template<typename T>
size_t FindNearestKeyframeIndex(const std::vector<T>& keyframes, float animationTime)
{
    if (keyframes.size() == 2) return 0;

    size_t i = 0;
    auto it = keyframes.begin();
    auto penult = keyframes.end() - 1;

    while (it != penult)
    {
        auto nextFrame = *(it + 1);
        if (animationTime < nextFrame.timeStamp) return i;
        it++;
        i++;
    }

    throw std::runtime_error("Failed to find keyframe");
}

/// Computes the ratio of the current time between two timestamps. For use with lerp.
float TimestampRatio(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    auto midWayLength = animationTime - lastTimeStamp;
    auto framesDiff = nextTimeStamp - lastTimeStamp;
    return midWayLength / framesDiff;
}

template<typename T>
T Lerp(const T& x, const T& y, float a)
{
    return x * (1.0f - a) + y * a;
}

aiVector3D Bone::InterpolatePosition(float animationTime)
{
    auto lerpedPosition = positions[0].position;

    // Can only interpolate if we have more than one keyframe, otherwise just use the one
    if (positions.size() > 1)
    {
        // Get the current and next keyframes
        auto idx0 = FindNearestKeyframeIndex(positions, animationTime);
        auto frame0 = positions[idx0];
        auto frame1 = positions[idx0 + 1];

        // Interpolate between them
        auto scaleFactor = TimestampRatio(frame0.timeStamp, frame1.timeStamp, animationTime);
        lerpedPosition = Lerp(frame0.position, frame1.position, scaleFactor);
    }

    return lerpedPosition;
}

aiQuaternion Bone::InterpolateRotation(float animationTime)
{
    auto lerpedRotation = rotations[0].orientation;

    if (rotations.size() > 1)
    {
        auto idx0 = FindNearestKeyframeIndex(rotations, animationTime);
        auto frame0 = rotations[idx0];
        auto frame1 = rotations[idx0 + 1];

        auto scaleFactor = TimestampRatio(frame0.timeStamp, frame1.timeStamp, animationTime);
        aiQuaternion::Interpolate(lerpedRotation, frame0.orientation, frame1.orientation, scaleFactor);
    }
    
    return lerpedRotation.Normalize();
}

aiVector3D Bone::InterpolateScaling(float animationTime)
{
    auto lerpedScale = scales[0].scale;

    if (scales.size() > 1)
    {
        auto idx0 = FindNearestKeyframeIndex(scales, animationTime);
        auto frame0 = scales[idx0];
        auto frame1 = scales[idx0 + 1];

        auto scaleFactor = TimestampRatio(frame0.timeStamp, frame1.timeStamp, animationTime);
        lerpedScale = Lerp(frame0.scale, frame1.scale, scaleFactor);
    }

    return lerpedScale;
}

#endif // USE_NEWGFX
