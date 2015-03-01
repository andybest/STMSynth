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
#include "SynthiaUtils.h"


namespace Synthia
{
    enum
    {
        kMoogVoiceParameter_OSC1_Waveform,
        kMoogVoiceParameter_OSC1_Volume,
        kMoogVoiceParameter_OSC1_Tune,
        kMoogVoiceParameter_OSC1_FineTune,
        
        kMoogVoiceParameter_OSC2_Waveform,
        kMoogVoiceParameter_OSC2_Volume,
        kMoogVoiceParameter_OSC2_Tune,
        kMoogVoiceParameter_OSC2_FineTune,
        
        kMoogVoiceParameter_OSC3_Waveform,
        kMoogVoiceParameter_OSC3_Volume,
        kMoogVoiceParameter_OSC3_Tune,
        kMoogVoiceParameter_OSC3_FineTune,
        
        kMoogVoiceParameter_Envelope_Attack,
        kMoogVoiceParameter_Envelope_Decay,
        kMoogVoiceParameter_Envelope_Sustain,
        kMoogVoiceParameter_Envelope_Release,
    };
    
    class MoogVoice : public SynthVoice
    {
    public:
        void init(SynthContext *ctx);
        
        void calcFrequencies(float baseFreq);
        void setFrequency(float freq);
        float tick(int channel);
        
        void keyOn();
        void keyOff();
        
        void changeValueForControlId(ControlEntryId id, float value);
        void processPitchBend(int pitchBendValue);
        void setupControlEntries();
        
    private:
        SwitchableWavetableOscillator _osc1;
        SwitchableWavetableOscillator _osc2;
        SwitchableWavetableOscillator _osc3;
        
        float _osc1Volume;
        float _osc1Tune;
        float _osc1FineTune;
        
        float _osc2Volume;
        float _osc2Tune;
        float _osc2FineTune;
        
        float _osc3Volume;
        float _osc3Tune;
        float _osc3FineTune;
        
        int _pitchBend;
        
        Envelope _envelope;
        
    };
}
