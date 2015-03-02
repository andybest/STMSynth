//
//  SineOscillator.cpp
//  Callisto
//
//  Created by Andy Best on 16/06/2014.
//  Copyright (c) 2014 Andy Best. All rights reserved.
//

#include "SineOscillator.h"
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

namespace Synthia
{
    void SineOscillator::init(SynthContext *ctx)
    {
        this->_ctx = ctx;
        this->setFrequency(440.0f);
    }
    
    void SineOscillator::setFrequency(float freq)
    {
        this->_frequency = freq;
        _tickStep = ((M_PI * 2 * this->_frequency) / _ctx->sampleRate());
    }
    
    float SineOscillator::tick(int channel)
    {
        float samp = sinf(_phase);
        
        _phase += _tickStep;
        
        if(_phase > M_PI * 2)
            _phase -= M_PI * 2;
        
        return samp;
    }
}
