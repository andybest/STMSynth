/* 
 * File:   MidiTask.cpp
 * Author: andybest
 * 
 * Created on 22 February 2015, 16:00
 */

#include "MidiTask.h"
#include "usbd_midi.h"
#include "cmsis_os.h"


MidiTask::MidiTask() {
}

MidiTask::~MidiTask() {
}

std::string MidiTask::getName() {
    return std::string("Midi Task");
}

void MidiTask::init() {

}

bool MidiTask::run() {
    USB_MIDI_Event_Packet_TypeDef packet;

    // Wait for a MIDI message.
    if (xQueueReceive(midiQueueHandle, &packet, portMAX_DELAY)) {
        
        MidiMessage_t msg;
        
        msg.type = (packet.midi0 & 0xF0) >> 4;
        msg.channel = packet.midi0 & 0xF;
        
        switch(msg.type) {
            case MIDI_MESSAGE_NOTE_ON:
                msg.NoteOn.key = packet.midi1 & 127;
                msg.NoteOn.velocity = packet.midi2 & 127;
                
                printf("Midi Note On- channel: %u, key: %u, vel %u\r\n", 
                        msg.channel, 
                        msg.NoteOn.key, 
                        msg.NoteOn.velocity);
                break;
                
            case MIDI_MESSAGE_NOTE_OFF:
                msg.NoteOff.key = packet.midi1 & 127;
                msg.NoteOff.velocity = packet.midi2 & 127;
                
                printf("Midi Note Off- channel: %u, key: %u, vel %u\r\n", 
                        msg.channel, 
                        msg.NoteOff.key, 
                        msg.NoteOff.velocity);
                break;
                
            case MIDI_MESSAGE_AFTERTOUCH:
                msg.AfterTouch.key = packet.midi1 & 127;
                msg.AfterTouch.velocity = packet.midi2 & 127;
                
                printf("Midi Aftertouch- channel: %u, key: %u, vel %u\r\n", 
                        msg.channel, 
                        msg.AfterTouch.key, 
                        msg.AfterTouch.velocity);
                break;
                
            case MIDI_MESSAGE_CONTROL_CHANGE:
                msg.ControlChange.controllerNumber = packet.midi1 & 127;
                msg.ControlChange.controllerValue = packet.midi2 & 127;
                
                printf("Midi Control Change- channel: %u controller number: %u controller value: %u\r\n", 
                        msg.channel, 
                        msg.ControlChange.controllerNumber,
                        msg.ControlChange.controllerValue);
                break;
                
            case MIDI_MESSAGE_PITCH_BEND:
                uint16_t pitchBendUnsigned = 0;
                int16_t pitchBendSigned;
                pitchBendUnsigned += packet.midi1 & 127;
                pitchBendUnsigned += (packet.midi2 & 127) << 7;
                pitchBendSigned = pitchBendUnsigned - 0x2000;
                msg.PitchBend.pitchBendValue = pitchBendSigned;
                
                printf("Midi pitch bend- channel: %u pitch bend value: %hi\r\n",
                        msg.channel, 
                        msg.PitchBend.pitchBendValue);
                break;  
        }
        
        xQueueSendToBack(synthMidiQueueHandle, &msg, portMAX_DELAY);
        /*if (packet.codeIndexNumber == 0x9) {
            // Note On
            printf("MIDI Note On- key: %u vel %u\r\n", (unsigned int) packet.midi1, (unsigned int) packet.midi2);
        } else if (packet.codeIndexNumber == 0x8) {
            // Note Off
            printf("MIDI Note Off- key: %u vel %u\r\n", (unsigned int) packet.midi1, (unsigned int) packet.midi2);
        }*/
    }

    return true;
}

