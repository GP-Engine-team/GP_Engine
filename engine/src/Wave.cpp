#include "Engine/Resources/Wave.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/ECS/System/SystemsManager.hpp"
#define DR_WAV_IMPLEMENTATION
#include <dr_wav/dr_wav.h>

using namespace GPE;

Wave::Wave(const char* filepath, const char* name)
{
    drwav wav;
    if (!drwav_init_file(&wav, filepath, NULL))
    {
        FUNCT_ERROR("couldn't load wav");
    }

    drwav_int16* pSampleData = (drwav_int16*)malloc(wav.totalPCMFrameCount * wav.channels * sizeof(drwav_int16));
    size_t       numberOfSamplesActuallyDecoded = drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pSampleData);

    bitsPerSample = 16;
    channels      = static_cast<uint8_t>(wav.channels);
    sampleRate    = wav.sampleRate;
    data          = pSampleData;
    size          = wav.totalPCMFrameCount * wav.channels * sizeof(drwav_int16);

    AL_CALL(alGenBuffers, 1, &buffer.buffer);

    if (channels == 1 && bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if (channels == 1 && bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if (channels == 2 && bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if (channels == 2 && bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr << "ERROR: unrecognised wave format: " << channels << " channels, " << bitsPerSample
                  << " bps of files : " << name << std::endl;
    }

    AL_CALL(alBufferData, buffer.buffer, format, static_cast<const ALvoid*>(data), static_cast<ALsizei>(size),
            static_cast<ALsizei>(sampleRate));

    SystemsManager::getInstance()->resourceManager.add<Buffer>(name, buffer);

    drwav_uninit(&wav);
}
