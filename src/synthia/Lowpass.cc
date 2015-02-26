//
//  Lowpass.cpp
//  Synthia
//
//  Created by Andy Best on 18/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#include "Lowpass.h"
#include <math.h>
#include <stdio.h>

namespace Synthia
{   
    void Lowpass::init(SynthContext *ctx)
    {
        this->_ctx = ctx;
        in1 = in2 = in3 = in4 = 0.0f;
        out1 = out2 = out3 = out4 = 0.0f;
    }
    
    void Lowpass::setCutoff(float cutoff)
    {
        if (cutoff < 0) {
            cutoff = 0;
        }else if (cutoff > _ctx->sampleRate()) {
            cutoff = _ctx->sampleRate();
        }
        _cutoff = cutoff;
        printf("Cutoff: %f\n", _cutoff);
        //calcCoefficients();
    }
    
    void Lowpass::setResonance(float resonance)
    {
        if(resonance < 0) {
            resonance = 0;
        } else if(resonance > 4) {
            resonance = 4;
        }
        printf("Resonance: %f\n", _resonance);
        _resonance = resonance;
        //calcCoefficients();
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
        double f = _cutoff * 1.16;
        double fb = _resonance * (1.0 - 0.15 * f * f);
        input -= out4 * fb;
        input *= 0.35013 * (f*f)*(f*f);
        out1 = input + 0.3 * in1 + (1 - f) * out1; // Pole 1
        in1 = input;
        out2 = out1 + 0.3 * in2 + (1 - f) * out2; // Pole 2
        in2 = out1;
        out3 = out2 + 0.3 * in3 + (1 - f) * out3; // Pole 3
        in3 = out2;
        out4 = out3 + 0.3 * in4 + (1 - f) * out4; // Pole 4
        in4 = out3;
        return out4;
    }
}
