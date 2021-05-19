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
struct KeyFrame
{
public:
    std::vector<GPM::SplitTransform> bones;

public:
    KeyFrame() = default;

    static void blend(const KeyFrame& key1, const KeyFrame& key2, float alpha, KeyFrame& buffer);

    size_t getNbBones() const noexcept
    {
        return bones.size();
    }
};

class Animation
{
public:
    struct AssimpAnimationData
    {
        size_t nbFrames = 0;
        size_t nbBones  = 0;
    };

public:
    std::string           name;
    std::vector<KeyFrame> keyFrames;
    float                 duration = 5.f;

public:
    Animation()                       = default;
    Animation(const Animation& other) = default;
    Animation(Animation&& other)      = default;
    ~Animation()                      = default;
    Animation& operator=(Animation const& other) = default;
    Animation& operator=(Animation&& other) = default;

    /**
     * @brief 
     * @param playTimeRatio If 0, the animation is starting. If 1, the animation is ending.
     * @return 
    */
    const KeyFrame& getPreviousKeyFrame(float playTimeRatio) const;
    const KeyFrame& getNextKeyFrame(float playTimeRatio) const;
    void getKeyFrame(float playTimeRatio, KeyFrame& buffer) const;

    void loadAnimFromAssimp(const AssimpAnimationData& animData);

    float getPlayTimeRatio(float currentTime) const
    {
        return currentTime / duration;
    }

    size_t getNbBones() const noexcept
    {
        if (keyFrames.empty())
            return 0;
        else
            return keyFrames.front().getNbBones();
    }

    void draw();
};

} /*namespace GPE*/
