//
//  MoogVoice.h
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#pragma once

#include <stdio.h>

#include "Synthia.h"


namespace Synthia
{
    class MoogVoice : public SynthVoice
    {
    public:
        void init(SynthContext *ctx);
        
        void setFrequency(float freq);
        float tick(int channel);
        
        void keyOn();
        void keyOff();

        
    };
}
