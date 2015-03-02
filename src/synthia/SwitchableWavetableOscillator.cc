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
        
        for(unsigned int i=0; i < _wavetableOscillators.size(); i++)
        {
            (_wavetableOscillators[i])->setFrequency(_frequency);
        }
    }
    
    int SwitchableWavetableOscillator::addWavetable(const float *sampleArray, int len)
    {
        _wavetableOscillators.push_back(new WavetableOscillator());
        WavetableOscillator *osc = _wavetableOscillators.back();
        
        osc->init(_ctx);
        osc->loadWavetableFromArray(sampleArray, len, true);
        //osc->setInterpolationType(kWavetableInterpolationNone);
        
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
        _currentOsc = _wavetableOscillators[_selectedOscillator];
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
}