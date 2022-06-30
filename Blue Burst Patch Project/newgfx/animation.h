#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>
#include <assimp/matrix4x4.h>
#include "model.h"
#include "bone.h"

class AnimatedModel : public Model
{
private:
    class Animation
    {
    private:
        friend class AnimatedModel;

        std::string name;
        float duration;
        float ticksPerSecond;
        std::vector<Bone> bones;

        Animation(const aiAnimation* anim);
        void AddBone(const std::string& name, size_t id, const aiNodeAnim* channel);
        size_t FindBoneIndex(const std::string& name) const;
        Bone& GetBone(size_t index);
    };

    struct AnimationNode
    {
    public:
        std::string name;
        aiMatrix4x4 transformation;
        std::vector<AnimationNode> children;
    };

    AnimationNode rootAnimationNode;
    std::vector<Animation> animations;
    Animation* currentAnimation;
    float currentTime;
    bool looping;
    std::vector<aiMatrix4x4> finalBoneMatrices;
    std::unordered_map<std::string, BoneInfo> boneInfoMap;
    size_t boneCount;

public:
    AnimatedModel(const std::string& path);
    void UpdateAnimation();
    void ChangeAnimation(const std::string& name);
    void ChangeAnimation(size_t index);
    void AnimationLoopingEnabled(bool looping);
    /// How much time has passed since animation started
    float AnimationTime() const;
    /// How much of the animation has already played on a range from 0 to 1
    float AnimationRatio() const;
    /// Returns true when the argument matches the currently playing frame
    bool CurrentFrame(size_t frame) const;
    /// Returns true when the argument matches the currently playing frame with the timeline being scaled to a range of 0..1
    bool CurrentFrameRatio(float ratio) const;
    bool AnimationEnded() const;

private:
    void ReadAnimationNode(AnimationNode& dst, const aiNode* src);
    void ReadAnimations();
    void ComputeBoneTransform(const AnimationNode& node, const aiMatrix4x4& parentTransform);
};
