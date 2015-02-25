//
// Created by Andy Best on 11/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//


#include "Generator.h"
#include "Triggerable.h"

#ifndef __Envelope_H_
#define __Envelope_H_

namespace Synthia
{
    typedef enum {
        kEnvelopeIdle = -1,
        kEnvelopePhaseAttack = 0,
        kEnvelopePhaseDecay,
        kEnvelopePhaseSustain,
        kEnvelopePhaseRelease
    } EnvelopeState_t;

    class Envelope : public Generator
    {
    public:
        void init(SynthContext *ctx);
        float tick(int channel);
        void setAttackTime(float attackTime);
        void setDecayTime(float decayTime);
        void setReleaseTime(float releaseTime);
        void setSustainLevel(float sustainLevel);
        void keyOn();
        void keyOff();
        void setRetriggerTime(float retriggerTime);

    private:
        float _timePerTick;
        float _target;
        float _value;
        EnvelopeState_t _state;

        float _attackRate;
        float _decayRate;
        float _sustainLevel;
        float _releaseRate;
        float _releaseLevel;

        float _retriggerTime;
        int _retriggerCount;
        int _retriggerSamples;
        float _retriggerStep;
        
        bool _isReleasing;

        
    };
}

#endif //__Envelope_H_



