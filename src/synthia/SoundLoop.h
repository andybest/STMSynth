//
// Created by Andy Best on 19/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//


#include "Generator.h"
#include "SoundFile.h"

#ifndef __SoundLoop_H_
#define __SoundLoop_H_


namespace Synthia
{
    class SoundLoop : public Generator
    {

    public:
        void loadFile(string filePath);

        void init(SynthContext *ctx);

        float tick(int channel);

    private:
        SoundFile *_soundFile;
        int *sampIdx;
    };
}

#endif //__SoundLoop_H_
