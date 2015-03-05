//
//  Delay.h
//  TestTool
//
//  Created by Andy Best on 04/03/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#pragma once

#include "Effect.h"

#define SYNTHIA_DELAY_LENGTH 44100

namespace Synthia
{
    class Delay : public Effect
    {
    public:
        void init(SynthContext *ctx);
        
        void setDelayTime(float delayTime);
        void setFeedbackLevel(float feedbackLevel);
        void setDelayVolume(float delayVolume);
        
        inline float tick(int channel, float input)
        {
            float delaySamp = _delaySamples[_delayIdx];
            float samp = (delaySamp * _volume) + input;
            _delaySamples[_delayIdx] = (input + delaySamp) * _feedback;
            
            // Wrap around buffer.
            if(++_delayIdx >= _delayLen)
                _delayIdx = 0;
            
            return samp;
        }
        
    private:
        float _delaySamples[SYNTHIA_DELAY_LENGTH];
        float _delayTime;
        unsigned int _delayLen;
        float _feedback;
        float _volume;
        unsigned int _delayIdx;
    };
}
