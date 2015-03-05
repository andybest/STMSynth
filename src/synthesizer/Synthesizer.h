/* 
 * File:   Synthesizer.h
 * Author: andybest
 *
 * Created on 22 February 2015, 21:22
 */

#pragma once

#include <vector>
#include <map>
#include "stdint.h"

#include "SynthContext.h"
#include "SynthVoice.h"
#include "SynthiaUtils.h"
#include "WavetableOscillator.h"
#include "Envelope.h"
#include "wavetable_samples.h"
#include "midi.h"
#include "MoogVoice.h"
#include "Delay.h"

using namespace Synthia;

typedef enum {
    kKeyTransitionTypeLegatoLastPlayed,
    kKeyTransitionTypePolyphonic
}KeyTransitionType_t;

enum {
    kSynthesizerParameter_Filter_Cutoff,
    kSynthesizerParameter_Filter_Resonance,
    
    kSynthesizerParameter_FilterEnvelope_Attack,
    kSynthesizerParameter_FilterEnvelope_Decay,
    kSynthesizerParameter_FilterEnvelope_Sustain,
    kSynthesizerParameter_FilterEnvelope_Release,
    
    kSynthesizerParameter_Delay_Time,
    kSynthesizerParameter_Delay_Feedback,
    kSynthesizerParameter_Delay_Volume,
    
    kSynthesizerParameter_MasterVolume,
    
    kSynthesizerParameter_FilterEnvelope_Enable,
    kSynthesizerParameter_HeadphoneVolume,
};

void setHeadphoneVolume(int vol);

class Synthesizer : public Controllable {
public:
    Synthesizer(uint32_t sampleRate);
    ~Synthesizer();
    
    KeyTransitionType_t keyTransitionType();
    
    void processMidiMessage(MidiMessage_t *msg);
    void addVoice(SynthVoice *voice);
    void addVoiceCCMapping(ControlEntryId id, unsigned char ccControllerNum);
    void processControlChange(unsigned char controllerNumber, unsigned char value);
    void sendControlChangeToVoices(ControlEntryId entryId, unsigned char value);
    void processPitchBend(int pitchBendValue);
    void sendPitchBendToVoices(int pitchBendValue);
    
    void keyOn(MidiMessage_t *msg);
    void keyOff(MidiMessage_t *msg);
    
    void changeValueForControlId(ControlEntryId id, float value);
    
    inline float tick()
    {
        float voiceSamp = 0.0f;
        
        for(int i=0; i < _voices.size(); i++)
        {
            voiceSamp += _voices[i]->tick(0);
        }
    
        if(_enableFilterEnvelope) {
            float eTick = _filterEnvelope.tick(0);
            _lowpassFilter.setCutoff(eTick * _filterCutoffMax + 0.01);
        } else {
            _lowpassFilter.setCutoff(_filterCutoffMax);
        }
        
        float filteredSamp = _lowpassFilter.tick(0, voiceSamp);
        return _delay.tick(0, filteredSamp) * _masterVolume;
    }
    
    
private:
    SynthContext synthContext;
    
    Envelope _filterEnvelope;
    Lowpass _lowpassFilter;
    Synthia::Delay _delay;
    float _filterCutoffMax;
    float _masterVolume;
    bool _enableFilterEnvelope;
    int _pitchBend;
    
    std::vector<uint8_t> _keyStack;
    
    std::vector<SynthVoice *> _voices;
    std::vector<SynthVoice *> _availableVoices;
    // Voices that are currently being played (keyon)
    std::map<unsigned char, SynthVoice *> _playingVoices;
    
    std::map<unsigned char, ControlEntryId> _voiceCCMapping;
    std::map<unsigned char, ControlEntryId> _paramCCMapping;
    
    KeyTransitionType_t _keyTransitionType;

};

