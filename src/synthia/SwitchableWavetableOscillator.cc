//
//  SwitchableWavetableOscillator.cpp
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#include "SwitchableWavetableOscillator.h"
#include <math.h>

namespace Synthia
{
    void SwitchableWavetableOscillator::init(SynthContext *ctx)
    {
        _ctx = ctx;
        _selectedOscillator = NULL;
    }
    
    void SwitchableWavetableOscillator::setFrequency(float freq)
    {
        _frequency = freq;
        
        for(auto it = _wavetableOscillators.begin(); it != _wavetableOscillators.end(); ++it)
        {
            (*it)->setFrequency(_frequency);
        }
    }
    
    int SwitchableWavetableOscillator::addWavetable(float *sampleArray, int len)
    {
        _wavetableOscillators.push_back(new WavetableOscillator());
        WavetableOscillator *osc = _wavetableOscillators.back();
        
        osc->init(_ctx);
        osc->loadWavetableFromArray(sampleArray, len, true);
        
        return 0;
    }
    
    unsigned int SwitchableWavetableOscillator::numWavetables()
    {
        return (unsigned int)_wavetableOscillators.size();
    }
    
    void SwitchableWavetableOscillator::selectWavetable(int wavetableIdx)
    {
        if(wavetableIdx >= _wavetableOscillators.size())
            return;
        
        _selectedOscillator = wavetableIdx;
    }
    
    void SwitchableWavetableOscillator::selectWavetableFloat(float value)
    {
        if(value < 0)
            value = 0;
        if(value > 1)
            value = 1;
        
        int idx = (int)floorf(value * _wavetableOscillators.size());
        selectWavetable(idx);
    }
    
    float SwitchableWavetableOscillator::tick(int channel)
    {
        if (channel >= _wavetableOscillators.size()) {
            return 0.0f;
        }
        
        return _wavetableOscillators[_selectedOscillator]->tick(channel);
    }
}