//
//  MoogVoice.cpp
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#include "MoogVoice.h"

#include "wavetable_samples.h"

namespace Synthia {
    
    void MoogVoice::init(SynthContext *ctx)
    {
        _ctx = ctx;
        
        _osc1.init(_ctx);
        _osc2.init(_ctx);
        _osc3.init(_ctx);
        
        _osc1.addWavetable(wavetable_triangle, wavetable_triangle_len);
        _osc2.addWavetable(wavetable_triangle, wavetable_triangle_len);
        _osc3.addWavetable(wavetable_triangle, wavetable_triangle_len);
        
        _osc1.addWavetable(wavetable_ramp, wavetable_ramp_len);
        _osc2.addWavetable(wavetable_ramp, wavetable_ramp_len);
        _osc3.addWavetable(wavetable_ramp, wavetable_ramp_len);
        
        _osc1.addWavetable(wavetable_pulse, wavetable_pulse_len);
        _osc2.addWavetable(wavetable_pulse, wavetable_pulse_len);
        _osc3.addWavetable(wavetable_pulse, wavetable_pulse_len);
        
        _envelope.init(_ctx);
        _filterEnvelope.init(_ctx);
        _lowpassFilter.init(_ctx);
    }
    
    void MoogVoice::setFrequency(float freq)
    {
        _frequency = freq;
    }
    
    void MoogVoice::keyOn()
    {
        _envelope.keyOn();
        _filterEnvelope.keyOn();
    }
    
    void MoogVoice::keyOff()
    {
        _envelope.keyOff();
        _filterEnvelope.keyOff();
    }
    
    float MoogVoice::tick(int channel)
    {
        float oscSamp = _osc1.tick(channel) + _osc2.tick(channel) + _osc3.tick(channel);
        
        float filteredSamp = _lowpassFilter.tick(channel, oscSamp);
        
        float envelopeSamp = filteredSamp * _envelope.tick(channel);
        
        return envelopeSamp;
    }
    
    
    
    
}