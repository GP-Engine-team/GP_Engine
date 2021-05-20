/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <GPM/Transform.hpp>
#include <string>
#include <vector>

namespace GPE
{
/**
 * @brief Relative to parent bones
 */
struct DeltaBone
{
public:
    // Variations of the bone transform between multiples ticks.
    std::vector<GPM::Vec3> deltaPositions;
    std::vector<GPM::Quat> deltaRotations;
    std::vector<GPM::Vec3> deltaScales;

    // Keys are sorted, and present only 1 time.
    std::vector<float> positionsTimeKeys;
    std::vector<float> rotationsTimeKeys;
    std::vector<float> scalesTimeKeys;

    GPM::SplitTransform getSplitDeltaTransform(float time) const
    {
        GPM::SplitTransform split;

        auto posIt     = std::lower_bound(positionsTimeKeys.begin(), positionsTimeKeys.end(), time);
        split.position = deltaPositions[posIt - positionsTimeKeys.begin()];

        auto rotIt     = std::lower_bound(rotationsTimeKeys.begin(), rotationsTimeKeys.end(), time);
        split.rotation = deltaRotations[posIt - rotationsTimeKeys.begin()];

        auto scaleIt = std::lower_bound(scalesTimeKeys.begin(), scalesTimeKeys.end(), time);
        split.scale  = deltaScales[posIt - scalesTimeKeys.begin()];

        return split;
    }
};

/**
 * @brief Contains world transformations of the bones
 */
struct AnimRenderFrame
{
    std::vector<GPM::Mat4> worldBonesOffsetsTransform;
};

class Animation
{
public:
    struct CreateArgs
    {
        std::string animName;

        float duration = 5.f;

        std::vector<std::string> boneNames;
        std::vector<DeltaBone>   boneDeltas;
    };

public:
    std::string                                name;
    std::unordered_map<std::string, DeltaBone> boneVariations;
    float                                      duration = 5.f;

public:
    Animation()                       = default;
    Animation(const Animation& other) = default;
    Animation(Animation&& other)      = default;
    ~Animation()                      = default;
    Animation& operator=(Animation const& other) = default;
    Animation& operator=(Animation&& other) = default;

    ///**
    // * @brief
    // * @param playTimeRatio If 0, the animation is starting. If 1, the animation is ending.
    // * @return
    // */
    //const KeyFrame& getPreviousKeyFrame(float playTimeRatio) const;
    //const KeyFrame& getNextKeyFrame(float playTimeRatio) const;
    //void            getKeyFrame(float playTimeRatio, KeyFrame& buffer) const;

    //void loadAnimFromAssimp(const CreateArgs& animData);

    //float getPlayTimeRatio(float currentTime) const
    //{
    //    return currentTime / duration;
    //}

    size_t getNbBones() const noexcept
    {
        //if (keyFrames.empty())
        //    return 0;
        //else
        //    return keyFrames.front().getNbBones();

        return 0;
    }
};

} /*namespace GPE*/
