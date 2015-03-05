//
//  Delay.cpp
//  TestTool
//
//  Created by Andy Best on 04/03/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#include "Delay.h"
#include <stdio.h>

namespace Synthia {

    void Delay::init(SynthContext *ctx)
    {
        _ctx = ctx;
        setDelayTime(0.3);
        _delayIdx = 0;
        _feedback = 0.8;
        _volume = 0.6;
        
        _delaySamples = NULL;
        _delaySamples = (float *)malloc(sizeof(float) * SYNTHIA_DELAY_LENGTH);
        if(_delaySamples == NULL) {
            printf("Failed to allocate memory for delay!!!");
            return;
        }
        
        for(int i=0; i < SYNTHIA_DELAY_LENGTH; i++)
        {
            _delaySamples[i] = 0.0f;
        }
    }
    
    void Delay::setDelayTime(float delayTime)
    {
        _delayTime = delayTime;
        _delayLen = (unsigned int)(_delayTime * SYNTHIA_DELAY_LENGTH);
    }
    
    void Delay::setFeedbackLevel(float feedbackLevel)
    {
        _feedback = feedbackLevel;
    }
    
    void Delay::setDelayVolume(float delayVolume)
    {
        _volume = delayVolume;
    }
    
}