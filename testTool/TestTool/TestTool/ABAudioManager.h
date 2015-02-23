//
//  ABAudioManager.h
//  TestTool
//
//  Created by Andy Best on 23/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMIDI/CoreMIDI.h>

#import "RtMidi.h"
#import "Novocaine.h"
#import "Synthesizer.h"

using namespace Synthia;

@interface ABAudioManager : NSObject
{
    RtMidiIn *_midiIn;
    
    Synthesizer *_synth;
}

@property (nonatomic, strong) Novocaine *audioManager;

@end
