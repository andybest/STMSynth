//
//  WavetableOscillator.cpp
//  Callisto
//
//  Created by Andy Best on 22/06/2014.
//  Copyright (c) 2014 Andy Best. All rights reserved.
//

#include "WavetableOscillator.h"

#include <math.h>

using namespace std;

namespace Synthia {
    void WavetableOscillator::init(SynthContext *ctx) {
        _ctx = ctx;
        
        _baseFrequency = 440.0f;
        _accumulator = 0.0;
        _interpolationType = kWavetableInterpolationLinear;
    }
    
    void WavetableOscillator::loadWavetable(string filename, bool isSingleCycle)
    {
        if(!_soundFile)
        {
            _soundFile = new SoundFile();
        }
        
        _soundFile->loadWav(filename);
        _baseFrequency = _ctx->sampleRate() / _soundFile->length();
        if(isSingleCycle)
        {
            // Is single cycle, so we don't need to scale anything
            _freqScale = 1.0f;
        }else{
            // Scale so that unpitched sound is at middle C
            _freqScale = _baseFrequency / 261.625565f;
        }
        calculatePhaseStep();
    }
    
    void WavetableOscillator::setFrequency(float freq)
    {
        _freq = freq;
        calculatePhaseStep();
    }
    
    void WavetableOscillator::setInterpolationType(WavetableInterpolationType interpType)
    {
        _interpolationType = interpType;
        
        /*if(_interpolationType == kWavetableInterpolationNone)
            cout << "No interpolation" << endl;
        else
            cout << "Linear interpolation" << endl;*/
    }
    
    void WavetableOscillator::retrigger()
    {
        _accumulator = 0.0;
    }
    
    void WavetableOscillator::calculatePhaseStep()
    {
        _phaseStep = (_freq / _baseFrequency) * _freqScale;
    }
    
    float WavetableOscillator::tick(int channel)
    {
        float samp;
        
        float *samples = _soundFile->samples()[channel];
        
        if(_interpolationType == kWavetableInterpolationNone)
        {
            int sampleIdx = (int)_accumulator;
            samp = samples[sampleIdx];
        } else {
            // Interpolate between the 2 nearest samples to the accumulator
            int idxBase = (int)floor(_accumulator);
            double idxFrac = _accumulator - idxBase;
            
            // Get floor and ceil values
            float val = samples[idxBase];
            int idx2 = idxBase + 1;
            if(idx2 >= _soundFile->length())
                idx2 = 0;
            float val2 = samples[idx2];
            
            samp = val + ((val2 - val) * idxFrac);
        }
       
        _accumulator += _phaseStep;
        
        if(_accumulator > _soundFile->length()) {
            _accumulator -= _soundFile->length();
        }
        return samp;
    }
    
}
