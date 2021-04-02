/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <OpenAL_Soft_Debug.hpp>
#include <dr_wav/dr_wav.h>
#include <vector>

namespace GPE
{

class Sound
{
public:
    Sound()  = default;
    ~Sound() = default;

public:
    std::uint8_t channels      = 0;
    std::int32_t sampleRate    = 0;
    std::uint8_t bitsPerSample = 0;
    int16_t*     data          = nullptr;
    drwav_uint64 size          = 0;
    ALenum       format        = 0;

    struct Buffer
    {
        ALuint buffer = 0;
    };

    Buffer buffer;
};
} // namespace GPE
