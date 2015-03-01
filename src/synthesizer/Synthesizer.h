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
    
    kSynthesizerParameter_MasterVolume,
};

class Synthesizer : public Controllable {
public:
    Synthesizer(uint32_t sampleRate);
    ~Synthesizer();
    
    KeyTransitionType_t keyTransitionType();
    
    void processMidiMessage(MidiMessage_t *msg);
    float tick();
    void addVoice(SynthVoice *voice);
    void addVoiceCCMapping(ControlEntryId id, unsigned char ccControllerNum);
    void processControlChange(unsigned char controllerNumber, unsigned char value);
    void sendControlChangeToVoices(ControlEntryId entryId, unsigned char value);
    
    void changeValueForControlId(ControlEntryId id, float value);
    
    
private:
    SynthContext synthContext;
    
    Envelope _filterEnvelope;
    Lowpass _lowpassFilter;
    float _filterCutoffMax;
    float _masterVolume;
    
    std::vector<uint8_t> _keyStack;
    
    std::vector<SynthVoice *> _voices;
    std::vector<SynthVoice *> _availableVoices;
    // Voices that are currently being played (keyon)
    std::vector<SynthVoice *> _playingVoices;
    
    std::map<unsigned char, ControlEntryId> _voiceCCMapping;
    std::map<unsigned char, ControlEntryId> _paramCCMapping;
    
    KeyTransitionType_t _keyTransitionType;

};

