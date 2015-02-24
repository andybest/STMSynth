//
//  WavetableOscillator.h
//  Callisto
//
//  Created by Andy Best on 22/06/2014.
//  Copyright (c) 2014 Andy Best. All rights reserved.
//

#ifndef __Callisto__WavetableOscillator__
#define __Callisto__WavetableOscillator__

//#include <iostream>

#include "Generator.h"
#include "SoundFile.h"

namespace Synthia {
    
    typedef enum {
        kWavetableInterpolationNone,
        kWavetableInterpolationLinear
    } WavetableInterpolationType;
    
    class WavetableOscillator : public PitchedGenerator
    {
    public:
        void init(SynthContext *ctx);
        void setFrequency(float freq);
        void setInterpolationType(WavetableInterpolationType interpType);
        float tick(int channel);
        void doPrecalculation(bool isSingleCycle);
        void loadWavetable(string filename, bool isSingleCycle);
        void loadWavetableFromArray(float *wtArray, int len, bool isSingleCycle);
        void retrigger();
    private:
        void calculatePhaseStep();
        
        SoundFile *_soundFile;
        
        float *_sampleArray;
        int _numSamples;
        
        float _baseFrequency;
        float _freq;
        double _freqScale;
        double _phaseStep;
        double _accumulator;
        WavetableInterpolationType _interpolationType;
    };
}

#endif /* defined(__Callisto__WavetableOscillator__) */
