//
//  SwitchableWavetableOscillator.cpp
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#include "SwitchableWavetableOscillator.h"

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
            it->setFrequency(_frequency);
        }
    }
    
    int SwitchableWavetableOscillator::addWavetable(float *sampleArray, int len)
    {
        _wavetableOscillators.push_back(WavetableOscillator());
        WavetableOscillator *osc = &_wavetableOscillators.back();
        
        osc->init(_ctx);
        osc->loadWavetableFromArray(sampleArray, len, true);
        
        if(_selectedOscillator == NULL)
            _selectedOscillator = osc;
        
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
        
        _selectedOscillator = &_wavetableOscillators.at(wavetableIdx);
    }
    
    float SwitchableWavetableOscillator::tick(int channel)
    {
        if (!_selectedOscillator) {
            return 0.0f;
        }
        
        return _selectedOscillator->tick(channel);
    }
}