//
//  PulseOscillator.cpp
//  Callisto
//
//  Created by Andy Best on 16/06/2014.
//  Copyright (c) 2014 Andy Best. All rights reserved.
//

#include "PulseOscillator.h"

namespace Synthia
{
    void PulseOscillator::init(SynthContext *ctx)
    {
        this->_ctx = ctx;
        setFrequency(440.0f);
        setPulseWidth(0.5f);
    }
    
    void PulseOscillator::setFrequency(float freq)
    {
        this->_frequency = freq;
        _tickStep = _frequency / _ctx->sampleRate();
    }
    
    void PulseOscillator::setPulseWidth(float pulseWidth)
    {
        if(pulseWidth < 0)
            pulseWidth = 0;
        if(pulseWidth > 1)
            pulseWidth = 1;
        this->_pulseWidth = pulseWidth;
    }
    
    float PulseOscillator::tick(int channel)
    {
        float samp;
        
        if(_phase >= _pulseWidth)
            samp = 1.0f;
        else
            samp = 0.0f;
        
        _phase += _tickStep;
        
        if(_phase > 1)
            _phase -= 1;
        
        return samp;
    }
}