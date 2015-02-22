/* 
 * File:   MidiTask.h
 * Author: andybest
 *
 * Created on 22 February 2015, 16:00
 */

#pragma once
#include "cpp_task.h"
#include "midi.h"

extern QueueHandle_t midiQueueHandle;

class MidiTask : public Task
{
public:
    MidiTask();
    ~MidiTask();
    
    std::string getName();
    void init();
    bool run();
    
private:

};

