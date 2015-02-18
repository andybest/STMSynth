//
//  Lowpass.cpp
//  Synthia
//
//  Created by Andy Best on 18/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#include "Lowpass.h"
#include <math.h>

namespace Synthia
{   
    void Lowpass::init(SynthContext *ctx)
    {
        this->_ctx = ctx;
    }
    
    void Lowpass::setCutoff(float cutoff)
    {
        if (cutoff < 0) {
            cutoff = 0;
        }else if (cutoff > _ctx->sampleRate()) {
            cutoff = _ctx->sampleRate();
        }
        _cutoff = cutoff;
        calcCoefficients();
    }
    
    void Lowpass::setResonance(float resonance)
    {
        if(resonance < 0) {
            resonance = 0;
        } else if(resonance > 1) {
            resonance = 1;
        }
        _resonance = resonance;
        calcCoefficients();
    }
    
    void Lowpass::calcCoefficients()
    {
        float frequency = _cutoff / (_ctx->sampleRate()/2.0f);
        q = 1.0f - frequency;
        p = frequency + 0.8f * frequency * q;
        f = p + p - 1.0f;
        q = _resonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));
    }
    
    float Lowpass::tick(int channel, float input)
    {
        input -= q * b4;                          //feedback
        t1 = b1;  b1 = (input + b0) * p - b1 * f;
        t2 = b2;  b2 = (b1 + t1) * p - b2 * f;
        t1 = b3;  b3 = (b2 + t2) * p - b3 * f;
                  b4 = (b3 + t1) * p - b4 * f;
        b4 = b4 - b4 * b4 * b4 * 0.166667f;    //clipping
        b0 = input;
        
        return b4;
    }
}
