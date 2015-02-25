//
//  Controllable.cpp
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#include "Controllable.h"

namespace Synthia
{
    
    bool Controllable::addControlEntry(ControlEntryId id, string name, ControlType type, float minValue, float maxValue)
    {
        ControlEntry entry;
        entry.id = id;
        entry.name = name;
        entry.type = type;
        
        if(_controlEntries.count(id) > 0)
        {
            // This ID already exists!
            return false;
        }
        
        _controlEntries[id] = entry;
        return true;
    }
    
    bool Controllable::setControlWithMidiCCValue(ControlEntryId id, unsigned char value)
    {
        // An entry with the id given does not exist
        if(_controlEntries.count(id) == 0)
            return false;
        
        ControlEntry entry = _controlEntries[id];
        
        convertFromMidiCCValue(value, entry);
        
        return true;
    }
    
    float Controllable::convertFromMidiCCValue(unsigned char value, ControlEntry entry)
    {
        switch (entry.type)
        {
            case kControlTypeFloatZeroOne:
                return (value & 0x7F) / 127.0f;
                
            case kControlTypeFloatCustomRange:
                float normalized = (value & 0x7F) / 127.0f;
                float difference = entry.rangeHigh - entry.rangeLow;
                return entry.rangeLow + (normalized * difference);
        }
        
        return 0.0f;
    }
    
    
}