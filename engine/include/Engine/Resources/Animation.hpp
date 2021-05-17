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
    float       duration = 5.f;

public:
    Animation()                      = default;
    Animation(const Animation& other) = default;
    Animation(Animation&& other)      = default;
    ~Animation()                     = default;
    Animation& operator=(Animation const& other) = default;
    Animation& operator=(Animation&& other) = default;
};

} /*namespace GPE*/
