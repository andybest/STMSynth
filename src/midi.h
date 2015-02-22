/* 
 * File:   midi.h
 * Author: andybest
 *
 * Created on 22 February 2015, 18:26
 */

#ifndef MIDI_H
#define	MIDI_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MIDI_GET_MESSAGE_TYPE(x)        x & 0xF0
#define MIDI_GET_MESSAGE_CHANNEL(x)     x & 0x0F
    
#define MIDI_MESSAGE_NOTE_OFF           0x8
#define MIDI_MESSAGE_NOTE_ON            0x9
#define MIDI_MESSAGE_AFTERTOUCH         0xA
#define MIDI_MESSAGE_CONTROL_CHANGE     0xB
#define MIDI_MESSAGE_PROGRAM_CHANGE     0xC
#define MIDI_MESSAGE_CHANNEL_PRESSURE   0xD
#define MIDI_MESSAGE_PITCH_BEND         0xE
    
    typedef struct {
        uint8_t channel:4;
        uint8_t type:4;
        
        union {
            struct {
                uint8_t velocity: 7;
                uint8_t key: 7;
            }NoteOn;
            
            struct {
                uint8_t velocity: 7;
                uint8_t key: 7;
            }NoteOff;
            
            struct {
                uint8_t velocity: 7;
                uint8_t key: 7;
            }AfterTouch;
            
            struct {
                uint8_t controllerValue: 7;
                uint8_t controllerNumber: 7;
            }ControlChange;
            
            struct {
                uint8_t programNumber;
            }ProgramChange;
            
            struct {
                uint8_t velocity;
            }ChannelPressure;
            
            struct {
                int16_t pitchBendValue;
            }PitchBend;
        };
    }MidiMessage_t;

#ifdef	__cplusplus
}
#endif

#endif	/* MIDI_H */

