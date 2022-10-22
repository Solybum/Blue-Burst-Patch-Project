#include "pch.h"
#ifdef USE_NEWGFX

#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <cmath>
#include "animation.h"
#include "../common.h"

// Game runs at 30 fps
const float DELTA_TIME = 1.0 / 30.0;

AnimatedModel::Animation::Animation(const aiAnimation* anim)
{
    name = anim->mName.C_Str();
    duration = anim->mDuration;

    ticksPerSecond = anim->mTicksPerSecond;
    if (ticksPerSecond == 0.0)
    {
        // 25-ish seems to be a common value
        ticksPerSecond = 25.0;
    }
}

void AnimatedModel::Animation::AddBone(const std::string& name, size_t id, const aiNodeAnim* channel)
{
    bones.emplace_back(name, id, channel);
}

size_t AnimatedModel::Animation::FindBoneIndex(const std::string& name) const
{
    for (size_t i = 0; i < bones.size(); i++)
    {
        if (bones[i].GetName() == name) return i;
    }

    return -1;
}

Bone& AnimatedModel::Animation::GetBone(size_t index)
{
    return bones[index];
}

AnimatedModel::AnimatedModel(const std::string& path) :
    currentTime(0.0),
    looping(true),
    boneCount(0),
    Model(path)
{
    if (scene->mNumAnimations == 0)
        throw std::runtime_error("File is missing animations: " + path);

    // Re-process meshes to add bone data for them
    for (auto& mesh : meshes)
    {
        auto meshData = scene->mMeshes[mesh.SceneMeshIndex()];

        std::vector<std::vector<VertexBoneData>> vertexBoneMap(mesh.VertexCount());

        // Write BoneInfos into boneInfoMap and create a mapping that allows each vertex to look up which bones are affecting it
        for (auto boneIndex = 0; boneIndex < meshData->mNumBones; boneIndex++)
        {
            auto boneName = std::string(meshData->mBones[boneIndex]->mName.C_Str());

            size_t boneId;

            // Find existing BoneInfo or create new
            auto entry = boneInfoMap.find(boneName);
            if (entry == boneInfoMap.end())
            {
                BoneInfo newBoneInfo;
                newBoneInfo.id = boneCount++;
                newBoneInfo.offset = meshData->mBones[boneIndex]->mOffsetMatrix;

                boneInfoMap[boneName] = newBoneInfo;

                boneId = newBoneInfo.id;
            }
            else
            {
                boneId = (*entry).second.id;
            }

            auto weights = meshData->mBones[boneIndex]->mWeights;

            // Save which vertices are affected by this bone
            for (auto weightIndex = 0; weightIndex < meshData->mBones[boneIndex]->mNumWeights; weightIndex++)
            {
                VertexBoneData boneData;
                boneData.boneId = boneId;
                boneData.boneWeight = weights[weightIndex].mWeight;
                vertexBoneMap[weights[weightIndex].mVertexId].push_back(boneData);
            }
        }

        mesh.SetVertexBoneMap(vertexBoneMap);
    }

    // Read animation nodes
    ReadAnimationNode(rootAnimationNode, scene->mRootNode);
    
    // Read animations
    for (auto i = 0; i < scene->mNumAnimations; i++)
    {
        auto animData = scene->mAnimations[i];
        auto anim = Animation(animData);

        for (auto i = 0; i < animData->mNumChannels; i++)
        {
            auto channel = animData->mChannels[i];
            // Channel name matches bone name
            auto boneName = std::string(channel->mNodeName.C_Str());

            // Sometimes there are channels that don't match any name in mesh->mBones (?)
            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                boneInfoMap[boneName].id = boneCount++;
            }

            anim.AddBone(boneName, boneInfoMap[boneName].id, channel);
        }

        animations.push_back(anim);
    }

    // Ensure there is one matrix for each bone
    finalBoneMatrices.resize(boneCount);

    ChangeAnimation(0);
}

/// Copy Assimp's nodes into a nicer format
void AnimatedModel::ReadAnimationNode(AnimationNode& dst, const aiNode* src)
{
    dst.name = std::string(src->mName.C_Str());
    dst.transformation = src->mTransformation;

    for (auto i = 0; i < src->mNumChildren; i++)
    {
        AnimationNode newNode;
        ReadAnimationNode(newNode, src->mChildren[i]);
        dst.children.push_back(newNode);
    }
}

void AnimatedModel::ReadAnimations()
{
    for (auto i = 0; i < scene->mNumAnimations; i++)
    {
        auto animData = scene->mAnimations[i];
        auto anim = Animation(animData);

        for (auto i = 0; i < animData->mNumChannels; i++)
        {
            auto channel = animData->mChannels[i];
            // Channel name matches bone name
            auto boneName = std::string(channel->mNodeName.C_Str());

            // Sometimes there are channels that don't match any name in mesh->mBones (?)
            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                boneInfoMap[boneName].id = boneCount++;
            }

            anim.AddBone(boneName, boneInfoMap[boneName].id, channel);
        }

        animations.push_back(anim);
    }
}

void AnimatedModel::UpdateAnimation()
{
    if (currentAnimation == nullptr) return;

    if (AnimationEnded())
    {
        // Previous update caused animation to end
        if (looping)
        {
            // Loop back to start
            currentTime = fmod(currentTime, currentAnimation->duration);
        }
        else
        {
            // Nothing to do
            return;
        }
    }

    ComputeBoneTransform(rootAnimationNode, aiMatrix4x4());

    for (auto& mesh : meshes)
    {
        mesh.ApplyBoneTransformations(finalBoneMatrices);
    }

    // Increment animation timer
    currentTime += currentAnimation->ticksPerSecond * DELTA_TIME;
}

void AnimatedModel::ChangeAnimation(const std::string& name)
{
    currentTime = 0.0;
    currentAnimation = nullptr;

    // Find by name
    for (auto i = 0; i < animations.size(); i++)
    {
        if (animations[i].name == name)
        {
            currentAnimation = &animations[i];
        }
    }
}

void AnimatedModel::ChangeAnimation(size_t index)
{
    currentTime = 0.0;
    currentAnimation = nullptr;

    if (index < animations.size())
    {
        currentAnimation = &animations[index];
    }
}

void AnimatedModel::ComputeBoneTransform(const AnimationNode& node, const aiMatrix4x4& parentTransform)
{
    auto nodeTransform = node.transformation;

    auto boneIndex = currentAnimation->FindBoneIndex(node.name);
    if (boneIndex != -1)
    {
        // Update bone
        auto& bone = currentAnimation->GetBone(boneIndex);
        bone.Update(currentTime);
        nodeTransform = bone.GetLocalTransform();
    }

    // Apply bone's transformation into parent's transformation
    auto globalTransform = parentTransform * nodeTransform;

    auto entry = boneInfoMap.find(node.name);
    if (entry != boneInfoMap.end())
    {
        // Save bone transformation
        auto boneInfo = (*entry).second;
        finalBoneMatrices[boneInfo.id] = globalTransform * boneInfo.offset;
    }

    for (const auto& child : node.children)
    {
        ComputeBoneTransform(child, globalTransform);
    }
}

void AnimatedModel::AnimationLoopingEnabled(bool loop)
{
    looping = loop;
}

float AnimatedModel::AnimationTime() const
{
    return currentTime;
}

float AnimatedModel::AnimationRatio() const
{
    if (currentAnimation == nullptr || currentAnimation->duration == 0.0) return 0.0;
    return currentTime / currentAnimation->duration;
}

bool AnimatedModel::CurrentFrame(size_t frame) const
{
    if (currentAnimation == nullptr) return false;

    auto currentFrame = size_t(currentTime / (currentAnimation->ticksPerSecond * DELTA_TIME));
    return frame >= currentFrame && frame < currentFrame + 1;
}

bool AnimatedModel::CurrentFrameRatio(float ratio) const
{
    if (currentAnimation == nullptr) return false;
    return CurrentFrame(size_t(ratio * currentAnimation->duration / (currentAnimation->ticksPerSecond * DELTA_TIME)));
}

bool AnimatedModel::AnimationEnded() const
{
    if (currentAnimation == nullptr) return true;
    return currentTime >= currentAnimation->duration;
}

#endif // USE_NEWGFX
