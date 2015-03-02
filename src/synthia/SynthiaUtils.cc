//
//  SynthiaUtils.cpp
//  TestTool
//
//  Created by Andy Best on 28/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#include "SynthiaUtils.h"
#include <math.h>

namespace Synthia
{
    
    float SynthiaUtils::midiNoteToHz(unsigned int midiNote)
    {
        return 440.0f * powf( 2.0f, (midiNote - 69.0f) / 12.0f );
    }
    
    float SynthiaUtils::tuneFrequencyByCents(float frequency, float cents)
    {
        return frequency * powf(2, cents/1200.0f);
    }
    
    float SynthiaUtils::tuneFrequencyBySteps(float frequency, float steps)
    {
        return frequency * powf(2, steps/12.0f);
    }

}