//
//  SwitchableWavetableOscillator.h
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <vector>

#include "SynthContext.h"
#include "Generator.h"
#include "WavetableOscillator.h"

namespace Synthia
{
    class SwitchableWavetableOscillator : public PitchedGenerator
    {
    public:
        void init(SynthContext *ctx);
        void setFrequency(float freq);
        
        int addWavetable(const float *sampleArray, int len);
        void selectWavetable(int wavetableIdx);
        void selectWavetableFloat(float value);
        
        unsigned int numWavetables();
        
        inline float tick(int channel)
        {
            /*if (channel >= _wavetableOscillators.size()) {
                return 0.0f;
            }*/
            
            return _currentOsc->tick(channel);
            //return _wavetableOscillators[_selectedOscillator]->tick(channel);
        }
        
    private:
        unsigned int _selectedOscillator;
        WavetableOscillator *_currentOsc;
        std::vector<WavetableOscillator *> _wavetableOscillators;
        
    };
}

