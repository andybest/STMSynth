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
    if (xQueueReceive(midiQueueHandle, &packet, 0)) {
        if (packet.codeIndexNumber == 0x9) {
            // Note On
            printf("MIDI Note On- key: %u vel %u\r\n", (unsigned int) packet.midi1, (unsigned int) packet.midi2);
        } else if (packet.codeIndexNumber == 0x8) {
            // Note Off
            printf("MIDI Note Off- key: %u vel %u\r\n", (unsigned int) packet.midi1, (unsigned int) packet.midi2);
        }
    }

    return true;
}

