//
//  MoogVoice.cpp
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#include "MoogVoice.h"

namespace Synthia {
    
    void MoogVoice::init(SynthContext *ctx)
    {
        _ctx = ctx;
    }
    
    void MoogVoice::setFrequency(float freq)
    {
        _frequency = freq;
    }
    
    void MoogVoice::keyOn()
    {
        
    }
    
    void MoogVoice::keyOff()
    {
        
    }
    
    float MoogVoice::tick(int channel)
    {
        return 0;
    }
    
    
    
    
}