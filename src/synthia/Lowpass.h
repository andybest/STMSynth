//
// Created by Andy Best on 18/02/2015.
// Copyright (c) 2015 Andy Best. All rights reserved.
//


#include "SObject.h"
#include "SynthContext.h"

#ifndef __Lowpass_H_
#define __Lowpass_H_

namespace Synthia
{
    class Lowpass : public SObject
    {
    public:
        void init(SynthContext *ctx);
        float tick(int channel, float input);
        void setCutoff(float cutoff);
        void setResonance(float resonance);
        

    protected:
        SynthContext *_ctx;
        
    private:
        float _cutoff;
        float _resonance;
        
        float q, p, f;
        
        float t1, t2, t3, t4;
        float b0, b1, b2, b3, b4;
        
        void calcCoefficients();
    };
    
}


#endif //__Lowpass_H_
