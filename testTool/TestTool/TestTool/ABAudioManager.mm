//
//  ABAudioManager.m
//  TestTool
//
//  Created by Andy Best on 23/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#import "ABAudioManager.h"


@implementation ABAudioManager

- (id)init
{
    if((self = [super init]))
    {
        [self initMIDI];
        [self initAudio];
    }
    return self;
}

- (void)initMIDI
{
    _midiIn = new RtMidiIn();
    _midiIn->openVirtualPort();
}

- (void)initAudio
{
    self.audioManager = [Novocaine audioManager];
    
    _synth = new Synthesizer((int)_audioManager.samplingRate);
    
    Synthesizer *blockSynth = _synth;
    RtMidiIn *blockMidiIn = _midiIn;
    
    [_audioManager setOutputBlock:^(float *data, UInt32 numFrames, UInt32 numChannels) {
        BOOL done = NO;
        std::vector<unsigned char> message;
        
        while(!done) {
            blockMidiIn->getMessage(&message);
            
            if(message.size() == 0){
                done = YES;
            } else {
                MidiMessage_t msg;
                
                msg.type = (message[0] & 0xF0) >> 4;
                msg.channel = message[0] & 0xF;
                
                switch(msg.type) {
                    case MIDI_MESSAGE_NOTE_ON:
                        msg.NoteOn.key = message[1] & 127;
                        msg.NoteOn.velocity = message[2] & 127;
                        
                        printf("Midi Note On- channel: %u, key: %u, vel %u\r\n",
                               msg.channel,
                               msg.NoteOn.key,
                               msg.NoteOn.velocity);
                        break;
                        
                    case MIDI_MESSAGE_NOTE_OFF:
                        msg.NoteOff.key = message[1] & 127;
                        msg.NoteOff.velocity = message[2] & 127;
                        
                        printf("Midi Note Off- channel: %u, key: %u, vel %u\r\n",
                               msg.channel,
                               msg.NoteOff.key,
                               msg.NoteOff.velocity);
                        break;
                        
                    case MIDI_MESSAGE_AFTERTOUCH:
                        msg.AfterTouch.key = message[1] & 127;
                        msg.AfterTouch.velocity = message[2] & 127;
                        
                        printf("Midi Aftertouch- channel: %u, key: %u, vel %u\r\n",
                               msg.channel,
                               msg.AfterTouch.key,
                               msg.AfterTouch.velocity);
                        break;
                        
                    case MIDI_MESSAGE_CONTROL_CHANGE:
                        msg.ControlChange.controllerNumber = message[1] & 127;
                        msg.ControlChange.controllerValue = message[2] & 127;
                        
                        printf("Midi Control Change- channel: %u controller number: %u controller value: %u\r\n",
                               msg.channel,
                               msg.ControlChange.controllerNumber,
                               msg.ControlChange.controllerValue);
                        break;
                        
                    case MIDI_MESSAGE_PITCH_BEND:
                        uint16_t pitchBendUnsigned = 0;
                        int16_t pitchBendSigned;
                        pitchBendUnsigned += message[1] & 127;
                        pitchBendUnsigned += (message[2] & 127) << 7;
                        pitchBendSigned = pitchBendUnsigned - 0x2000;
                        msg.PitchBend.pitchBendValue = pitchBendSigned;
                        
                        printf("Midi pitch bend- channel: %u pitch bend value: %hi\r\n",
                               msg.channel, 
                               msg.PitchBend.pitchBendValue);
                        break;  
                }
                
                blockSynth->processMidiMessage(&msg);
            }
            
            
        }
        
        bool clipped = NO;
        
        for(int i = 0; i < numFrames; i++) {
            float samp = blockSynth->tick();
            
            if(samp > 1.0f || samp < -1.0f)
                clipped = YES;
            float clipped = 0.5 * (fabs(samp + 1.0f) - fabs(samp - 1.0f)); // Branchless clipping
            float shaped = 1.5 * clipped - 0.5 * clipped * clipped * clipped; // Waveshaper.
            
            *data++ = shaped;
            *data++ = shaped;
        }
        
        if(clipped)
            NSLog(@"Clipped!");
        
    }];
    
    [_audioManager play];
    
}

@end
