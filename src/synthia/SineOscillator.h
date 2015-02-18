//
//  SineOscillator.h
//  Callisto
//
//  Created by Andy Best on 16/06/2014.
//  Copyright (c) 2014 Andy Best. All rights reserved.
//

#include "Generator.h"

namespace Synthia {
    
    class SineOscillator : public PitchedGenerator
    {
    public:
        void init(SynthContext *ctx);
        void setFrequency(float freq);
        float tick(int channel);
        
    private:
        float _phase;
        float _tickStep;
    };
}