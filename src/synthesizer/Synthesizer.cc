/* 
 * File:   Synthesizer.cc
 * Author: andybest
 * 
 * Created on 22 February 2015, 21:22
 */

#include "Synthesizer.h"
#include <math.h>

Synthesizer::Synthesizer(uint32_t sampleRate) : synthContext(sampleRate) {
    _filterEnvelope.init(&synthContext);
    _lowpassFilter.init(&synthContext);
    
    _masterVolume = 1.0f;
    
    addVoice(new MoogVoice());
    
    addVoiceCCMapping(kMoogVoiceParameter_OSC1_Waveform,    30);
    addVoiceCCMapping(kMoogVoiceParameter_OSC1_Volume,      31);
    addVoiceCCMapping(kMoogVoiceParameter_OSC1_Tune,        32);
    addVoiceCCMapping(kMoogVoiceParameter_OSC1_FineTune,    33);
    addVoiceCCMapping(kMoogVoiceParameter_OSC2_Waveform,    34);
    addVoiceCCMapping(kMoogVoiceParameter_OSC2_Volume,      35);
    addVoiceCCMapping(kMoogVoiceParameter_OSC2_Tune,        36);
    addVoiceCCMapping(kMoogVoiceParameter_OSC2_FineTune,    37);
    addVoiceCCMapping(kMoogVoiceParameter_OSC3_Waveform,    38);
    addVoiceCCMapping(kMoogVoiceParameter_OSC3_Volume,      39);
    addVoiceCCMapping(kMoogVoiceParameter_OSC3_Tune,        40);
    addVoiceCCMapping(kMoogVoiceParameter_OSC3_FineTune,    41);
    addVoiceCCMapping(kMoogVoiceParameter_Envelope_Attack,  42);
    addVoiceCCMapping(kMoogVoiceParameter_Envelope_Decay,   43);
    addVoiceCCMapping(kMoogVoiceParameter_Envelope_Sustain, 44);
    addVoiceCCMapping(kMoogVoiceParameter_Envelope_Release, 45);
    
    addControlEntry(kSynthesizerParameter_Filter_Cutoff, "Filter Cutoff", kControlTypeFloatCustomRange , 0.05f, 1.0f);
    addControlEntry(kSynthesizerParameter_Filter_Resonance, "Filter Resonance", kControlTypeFloatCustomRange, 0.0f, 4.0f);
    
    addControlEntry(kSynthesizerParameter_FilterEnvelope_Attack, "Filter Envelope Attack", kControlTypeFloatCustomRange, 0.0f, 2.0f);
    addControlEntry(kSynthesizerParameter_FilterEnvelope_Decay, "Filter Envelope Decay", kControlTypeFloatCustomRange, 0.0f, 2.0f);
    addControlEntry(kSynthesizerParameter_FilterEnvelope_Sustain, "Filter Envelope Sustain", kControlTypeFloatZeroOne);
    addControlEntry(kSynthesizerParameter_FilterEnvelope_Release, "Filter Envelope Release", kControlTypeFloatCustomRange, 0.0f, 2.0f);
    
    addControlEntry(kSynthesizerParameter_MasterVolume, "Master Volum", kControlTypeFloatCustomRange, 0.0f, 2.0f);
    
    _paramCCMapping[46] = kSynthesizerParameter_Filter_Cutoff;
    _paramCCMapping[47] = kSynthesizerParameter_Filter_Resonance;
    
    _paramCCMapping[48] = kSynthesizerParameter_FilterEnvelope_Attack;
    _paramCCMapping[49] = kSynthesizerParameter_FilterEnvelope_Decay;
    _paramCCMapping[50] = kSynthesizerParameter_FilterEnvelope_Sustain;
    _paramCCMapping[51] = kSynthesizerParameter_FilterEnvelope_Release;
    
    _paramCCMapping[52] = kSynthesizerParameter_MasterVolume;
  
    
    //_lowpassFilter.setResonance(0.5);
    _filterCutoffMax = 1.0f;
    _filterEnvelope.setAttackTime(0.2);
    _filterEnvelope.setDecayTime(0.0);
    _filterEnvelope.setSustainLevel(1.0);
    _filterEnvelope.setReleaseTime(1.0);
}

Synthesizer::~Synthesizer() {
}

float Synthesizer::midiNoteToFrequency(uint32_t note)
{
    return 440.0f * powf( 2.0f, (note - 69.0f) / 12.0f );
}

void Synthesizer::addVoice(SynthVoice *voice)
{
    voice->init(&synthContext);
    _voices.push_back(voice);
    _availableVoices.push_back(_voices.back());
}

void Synthesizer::addVoiceCCMapping(ControlEntryId id, unsigned char ccControllerNum)
{
    if(_voiceCCMapping.count(ccControllerNum) > 0)
        return;
    
    _voiceCCMapping[ccControllerNum] = id;
    
    ControlEntryId savedId = _voiceCCMapping[ccControllerNum];
    
}

void Synthesizer::processMidiMessage(MidiMessage_t *msg)
{
    if(msg->type == MIDI_MESSAGE_NOTE_ON) {
        if(_keyStack.size() == 0) {
            _voices.back()->keyOn();
            _filterEnvelope.keyOn();
        }
        //osc1.setFrequency(midiNoteToFrequency(msg->NoteOn.key));
        _voices.back()->setFrequency(midiNoteToFrequency(msg->NoteOn.key));
        _keyStack.push_back(msg->NoteOn.key);
    } else if(msg->type == MIDI_MESSAGE_NOTE_OFF) {
        int idx = -1;
        for(uint32_t i = 0; i < _keyStack.size(); i++) {
            if(_keyStack.at(i) == msg->NoteOff.key) {
                idx = i;
                break;
            }
        }
        
        if(idx >= 0) {
            std::vector<uint8_t>::iterator toDelete = _keyStack.begin() + idx;
            _keyStack.erase(toDelete);
            
            if(_keyStack.size() == 0) {
                //envelope.keyOff();
                _filterEnvelope.keyOff();
                _voices.back()->keyOff();
            } else {
                _voices.back()->setFrequency(midiNoteToFrequency(_keyStack.at(_keyStack.size() - 1)));
                //osc1.setFrequency(midiNoteToFrequency(_keyStack.at(_keyStack.size() - 1)));
            }
        }
    } else if(msg->type == MIDI_MESSAGE_CONTROL_CHANGE) {
        processControlChange(msg->ControlChange.controllerNumber, msg->ControlChange.controllerValue);
    }
}

void Synthesizer::processControlChange(unsigned char controllerNumber, unsigned char value)
{
    if(_voiceCCMapping.count(controllerNumber) > 0) {
        ControlEntryId id = _voiceCCMapping[controllerNumber];
        sendControlChangeToVoices(id, value);
        return;
    }
    
    if(_paramCCMapping.count(controllerNumber) > 0) {
        setControlWithMidiCCValue(_paramCCMapping[controllerNumber], value);
    }
}

void Synthesizer::sendControlChangeToVoices(ControlEntryId entryId, unsigned char value)
{
    for(auto voice = _voices.begin(); voice != _voices.end(); ++voice)
    {
        (*voice)->setControlWithMidiCCValue(entryId, value);
    }
}

float Synthesizer::tick()
{
    float voiceSamp = 0.0f;
    for(auto v = _voices.begin(); v != _voices.end(); ++v)
    {
        MoogVoice *voice = static_cast<MoogVoice *>(*v);
        voiceSamp += voice->tick(0);
    }
    
    float eTick = _filterEnvelope.tick(0);
    _lowpassFilter.setCutoff(eTick * _filterCutoffMax + 0.01);
    float filteredSamp = _lowpassFilter.tick(0, voiceSamp);
    
    return filteredSamp * _masterVolume;
}

void Synthesizer::changeValueForControlId(ControlEntryId id, float value) {
    switch(id)
    {
            /* FILTER */
        case kSynthesizerParameter_Filter_Cutoff:
            //_lowpassFilter.setCutoff(value);
            _filterCutoffMax = value;
            break;
            
        case kSynthesizerParameter_Filter_Resonance:
            _lowpassFilter.setResonance(value);
            break;
            
            /* FILTER ENVELOPE */
        case kSynthesizerParameter_FilterEnvelope_Attack:
            _filterEnvelope.setAttackTime(value);
            break;
            
        case kSynthesizerParameter_FilterEnvelope_Decay:
            _filterEnvelope.setDecayTime(value);
            break;
            
        case kSynthesizerParameter_FilterEnvelope_Sustain:
            _filterEnvelope.setSustainLevel(value);
            break;
            
        case kSynthesizerParameter_FilterEnvelope_Release:
            _filterEnvelope.setReleaseTime(value);
            break;
            
        case kSynthesizerParameter_MasterVolume:
            _masterVolume = value;
            break;
    }
}

