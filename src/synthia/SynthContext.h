//
// Created by Andy Best on 11/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//


#include "SObject.h"

#ifndef __SynthContext_H_
#define __SynthContext_H_


namespace Synthia
{
    class SynthContext : public SObject
    {
    public:
        SynthContext(float sampleRate) { setSampleRate(sampleRate); }
        float sampleRate() { return _sampleRate; }
        void setSampleRate(float sr) { _sampleRate = sr; }
    private:
        float _sampleRate;
    };
}


#endif //__SynthContext_H_
