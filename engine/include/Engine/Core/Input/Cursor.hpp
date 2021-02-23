/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once
#include "GPM/Vector2.hpp"

namespace GPE
{
class Cursor
{
public:
    Cursor()  = default;
    ~Cursor() = default;

public:
    GPM::Vec2 position         = {300, 450};
    GPM::Vec2 deltaDisplasment = {0, 0};
    //GPM::Vec2
};
} // namespace GPE