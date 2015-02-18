//
// Created by Andy Best on 19/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//

#include "SoundLoop.h"

namespace Synthia
{
    void SoundLoop::init(SynthContext *ctx)
    {
        _ctx = ctx;
        sampIdx = 0;
    }

    void SoundLoop::loadFile(string filePath)
    {
        _soundFile = new SoundFile();
        _soundFile->loadWav(filePath);
        sampIdx = new int[_soundFile->channels()];
    }

    float SoundLoop::tick(int channel)
    {
        float samp = _soundFile->samples()[channel][sampIdx[channel]];

        sampIdx[channel]++;
        if(sampIdx[channel] >= _soundFile->length() - 1)
            sampIdx[channel] -= _soundFile->length();
        return samp;
    }
}