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
#include <math.h>

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
        void doPrecalculation(bool isSingleCycle);
        void loadWavetable(string filename, bool isSingleCycle);
        void loadWavetableFromArray(const float *wtArray, int len, bool isSingleCycle);
        void retrigger();
        
        inline float tick(int channel)
        {
            float samp;
        
            if(_interpolationType == kWavetableInterpolationNone)
            {
                int sampleIdx = (int)_accumulator;
                samp = _sampleArray[sampleIdx];
            } else {
                // Interpolate between the 2 nearest samples to the accumulator
                int idxBase = (int)floor(_accumulator);
                float idxFrac = _accumulator - idxBase;
            
                // Get floor and ceil values
                float val = _sampleArray[idxBase];
                int idx2 = idxBase + 1;
                if(idx2 >= _numSamples)
                    idx2 = 0;
                float val2 = _sampleArray[idx2];
            
                samp = val + ((val2 - val) * idxFrac);
            }
       
            _accumulator += _phaseStep;
        
            if(_accumulator > _numSamples) {
                _accumulator -= _numSamples;
            }
            return samp;
        }
        
        
    private:
        void calculatePhaseStep();
        
        SoundFile *_soundFile;
        
        const float *_sampleArray;
        int _numSamples;
        float _invNumSamples;
        
        float _baseFrequency;
        float _invBaseFrequency;
        float _freq;
        float _freqScale;
        float _phaseStep;
        float _accumulator;
        WavetableInterpolationType _interpolationType;
    };
}

#endif /* defined(__Callisto__WavetableOscillator__) */
