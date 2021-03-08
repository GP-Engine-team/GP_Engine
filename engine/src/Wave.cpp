#include "Engine/Resources/Wave.hpp"
#define DR_WAV_IMPLEMENTATION
#include <dr_wav/dr_wav.h>

Wave::Wave(const char* filepath)
{
    drwav wav;
    if (!drwav_init_file(&wav, filepath, NULL))
    {
        abort();
        // LogError(SeverityEnum::Medium, __FUNCTION__, "couldn't load wav");
    }

    // short* pSampleData = (short*)malloc((unsigned int)wav.totalPCMFrameCount * wav.channels * sizeof(short));
    drwav_int16* pSampleData = (drwav_int16*)malloc(wav.totalPCMFrameCount * wav.channels * sizeof(drwav_int16));
    size_t       numberOfSamplesActuallyDecoded = drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pSampleData);

    bitsPerSample = 16;
    channels      = static_cast<uint8_t>(wav.channels);
    sampleRate    = wav.sampleRate;
    data          = pSampleData;
    size          = wav.totalPCMFrameCount * wav.channels * sizeof(drwav_int16);

    drwav_uninit(&wav);
}
