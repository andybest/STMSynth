//
//  Controllable.h
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace Synthia
{
    typedef unsigned int ControlEntryId;
    
    typedef enum
    {
        kControlTypeFloatZeroOne,
        kControlTypeFloatCustomRange,
        kControlTypeFloatZeroOneLogarithmic,
        kControlTypeFloatCustomRangeLogarithmic
    } ControlType;
    
    typedef struct
    {
        ControlType type;
        string name;
        ControlEntryId id;
        float rangeLow;
        float rangeHigh;
    } ControlEntry;
    
    class Controllable
    {
    public:
        bool addControlEntry(ControlEntryId id, string name, ControlType type, float minValue = 0, float maxValue = 1);
        bool setControlWithMidiCCValue(ControlEntryId id, unsigned char val);
        float convertFromMidiCCValue(unsigned char value, ControlEntry entry);
        
        virtual void changeValueForControlId(ControlEntryId id, float value) = 0;
        
    protected:
        map<ControlEntryId, ControlEntry> _controlEntries;
    };
}
