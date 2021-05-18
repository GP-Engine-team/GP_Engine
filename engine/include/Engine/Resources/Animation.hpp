/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

namespace GPE
{
class Animation
{
public:
    std::string name;
    std::vector<KeyFrame> keyFrames;
    float       duration = 5.f;

public:
    Animation()                      = default;
    Animation(const Animation& other) = default;
    Animation(Animation&& other)      = default;
    ~Animation()                     = default;
    Animation& operator=(Animation const& other) = default;
    Animation& operator=(Animation&& other) = default;

    KeyFrame& getPreviousKeyFrame();
    KeyFrame& getNextKeyFrame();
};

class KeyFrame
{
    std::vector<GPM::SplitTransform> bones;

    KeyFrame() = default;
    KeyFrame(const std::string& animName, int keyFrameIndex);

    void LoadAnim(const std::string& animName, int keyFrameIndex);

    static KeyFrame Blend(const KeyFrame& key1, const KeyFrame& key2, float alpha);
};

} /*namespace GPE*/
