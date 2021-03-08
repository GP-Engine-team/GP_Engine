#pragma once
#include <dr_wav/dr_wav.h>
#include <vector>

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
};
