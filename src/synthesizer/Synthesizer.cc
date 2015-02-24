/* 
 * File:   Synthesizer.cc
 * Author: andybest
 * 
 * Created on 22 February 2015, 21:22
 */

#include "Synthesizer.h"
#include <math.h>

Synthesizer::Synthesizer(uint32_t sampleRate) : synthContext(sampleRate) {
    osc1.init(&synthContext);
    osc1.loadWavetableFromArray(wavetable_triangle, wavetable_triangle_len, true);
    envelope.init(&synthContext);
    
    envelope.setAttackTime(0.08f);
    envelope.setReleaseTime(1.0f);
}

Synthesizer::~Synthesizer() {
}

float Synthesizer::midiNoteToFrequency(uint32_t note)
{
    return 440.0f * powf( 2.0f, (note - 69.0f) / 12.0f );
}

void Synthesizer::processMidiMessage(MidiMessage_t *msg)
{
    if(msg->type == MIDI_MESSAGE_NOTE_ON) {
        if(keyStack.size() == 0) {
            envelope.keyOn();
        }
        osc1.setFrequency(midiNoteToFrequency(msg->NoteOn.key));
        keyStack.push_back(msg->NoteOn.key);
    } else if(msg->type == MIDI_MESSAGE_NOTE_OFF) {
        int idx = -1;
        for(uint32_t i = 0; i < keyStack.size(); i++) {
            if(keyStack.at(i) == msg->NoteOff.key) {
                idx = i;
                break;
            }
        }
        
        if(idx >= 0) {
            std::vector<uint8_t>::iterator toDelete = keyStack.begin() + idx;
            keyStack.erase(toDelete);
            
            if(keyStack.size() == 0) {
                envelope.keyOff();
            } else {
                osc1.setFrequency(midiNoteToFrequency(keyStack.at(keyStack.size() - 1)));
            }
        }
    }
}

float Synthesizer::tick()
{
    float samp = osc1.tick(0) * envelope.tick(0);
    return samp;
}

