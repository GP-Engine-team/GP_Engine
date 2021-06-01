/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include "Sound.hpp"

namespace GPE
{
class Wave : public Sound
{
public:
    Wave() = default;
    Wave(const char* filepath, const char* name, const ALboolean spatialized = AL_FALSE);
    ~Wave() = default;
};
} // namespace GPE
