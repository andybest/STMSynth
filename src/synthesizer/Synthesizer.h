/* 
 * File:   Synthesizer.h
 * Author: andybest
 *
 * Created on 22 February 2015, 21:22
 */

#ifndef SYNTHESIZER_H
#define	SYNTHESIZER_H

#include <vector>
#include "stdint.h"

#include "SynthVoice.h"
#include "SynthContext.h"
#include "WavetableOscillator.h"
#include "wavetable_samples.h"
#include "Envelope.h"
#include "midi.h"

using namespace Synthia;

typedef enum {
    kKeyTransitionTypeLegatoLastPlayed,
    kKeyTransitionTypePolyphonic
}KeyTransitionType_t;

class Synthesizer {
public:
    Synthesizer(uint32_t sampleRate);
    ~Synthesizer();
    
    KeyTransitionType_t keyTransitionType();
    
    float midiNoteToFrequency(uint32_t note);
    void processMidiMessage(MidiMessage_t *msg);
    float tick();
    
    
private:
    SynthContext synthContext;
    WavetableOscillator osc1;
    Envelope envelope;
    
    std::vector<uint8_t> _keyStack;
    std::vector<SynthVoice *> _voices;
    
    KeyTransitionType_t _keyTransitionType;

};

#endif	/* SYNTHESIZER_H */

