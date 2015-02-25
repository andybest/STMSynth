//
// Created by Andy Best on 11/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//


#include "SObject.h"
#include "SynthContext.h"

#ifndef __Generator_H_
#define __Generator_H_

namespace Synthia
{
    class Generator : public SObject
    {
    public:
        virtual void init(SynthContext *ctx) = 0;
        virtual float tick(int channel) = 0;

    protected:
        SynthContext *_ctx;
    };
    
    
    class PitchedGenerator : public Generator
    {
    public:
        virtual void setFrequency(float freq) = 0;
    protected:
        float _frequency;
    };
}


#endif //__Generator_H_
