//
//  SynthiaUtils.h
//  TestTool
//
//  Created by Andy Best on 28/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#pragma once

#include <stdio.h>

namespace Synthia
{
    class SynthiaUtils
    {
        
    public:
        static float midiNoteToHz(unsigned int midiNote);
        static float tuneFrequencyByCents(float frequency, float cents);
        static float tuneFrequencyBySteps(float frequency, float steps);
    };
}
