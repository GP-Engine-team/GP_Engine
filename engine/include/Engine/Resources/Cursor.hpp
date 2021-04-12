/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once
#include "GPM/Vector2.hpp"

namespace GPE
{
struct Cursor
{
    GPM::Vec2 position = {0, 0};
    GPM::Vec2 deltaPos = {0, 0};
    GPM::Vec2 center   = {0, 0};
    bool      tracked  = false;
};
} // namespace GPE