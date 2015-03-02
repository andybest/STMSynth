//
// Created by Andy Best on 11/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//

#include "Envelope.h"

namespace Synthia
{
    void Envelope::init(SynthContext *ctx)
    {
        this->_ctx = ctx;

        _timePerTick = 1.0f / _ctx->sampleRate();

        _state = kEnvelopeIdle;
        _isReleasing = false;

        setAttackTime(0.1f);
        setDecayTime(0.5f);
        _sustainLevel = 1.0f;
        setReleaseTime(1.0f);

        setRetriggerTime(0.02f);
    }

    void Envelope::setAttackTime(float attackTime)
    {
        if(attackTime < 0.01)
            attackTime = 0.01;
        _attackRate = 1.0f / (attackTime / _timePerTick);
        
    }

    void Envelope::setDecayTime(float decayTime)
    {
        _decayRate = 1.0f / (decayTime / _timePerTick);
    }

    void Envelope::setReleaseTime(float releaseTime)
    {
        _releaseRate = 1.0f / (releaseTime / _timePerTick);
    }
    
    void Envelope::setSustainLevel(float sustainLevel)
    {
        _sustainLevel = sustainLevel;
    }

    inline float Envelope::tick(int channel) {
        switch(_state)
        {
            case kEnvelopeIdle:
                return 0.0f;
            case kEnvelopePhaseAttack:
            {
                // Retrigger ramp to prevent clicks
                if(_retriggerCount < _retriggerSamples)
                {
                    _value += _retriggerStep;
                    _retriggerCount ++;
                } else
                {
                    _value += _attackRate;
                    if (_value >= _target)
                    {
                        _value = _target;
                        _target = _sustainLevel;
                        _state = kEnvelopePhaseDecay;
                    }
                }
                break;
            }
            case kEnvelopePhaseDecay:
            {
                if(_isReleasing) _state = kEnvelopePhaseRelease;
                if ( _value > _sustainLevel ) {
                    _value -= _decayRate;
                    if ( _value <= _sustainLevel ) {
                        _value = _sustainLevel;
                        _state = kEnvelopePhaseSustain;
                    }
                }
                else {
                    _value += _decayRate; // attack target < sustain level
                    if ( _value >= _sustainLevel ) {
                        _value = _sustainLevel;
                        _state = kEnvelopePhaseSustain;
                    }
                }
                break;
            }
            case kEnvelopePhaseRelease:
            {
                _value -= _releaseRate;
                if ( _value <= 0.0 ) {
                    _value = 0.0;
                    _state = kEnvelopeIdle;
                }
                break;
            }
            case kEnvelopePhaseSustain:
            {
                if(_isReleasing) _state = kEnvelopePhaseRelease;
                break;
            }
        }

        return _value;
    }

    void Envelope::keyOn()
    {
        _state = kEnvelopePhaseAttack;
        _target = 1.0f;
        if(_retriggerSamples == 0)
            _value = 0.0f;
        _retriggerCount = 0;
        float retriggerTarget = _attackRate * _retriggerSamples;
        _retriggerStep = (retriggerTarget - _value) / _retriggerSamples;
        _isReleasing = false;
    }

    void Envelope::keyOff()
    {
        _isReleasing = true;//_state = kEnvelopePhaseRelease;
    }

    void Envelope::setRetriggerTime(float retriggerTime)
    {
        _retriggerTime = retriggerTime;
        _retriggerSamples = (int) (_retriggerTime / (1.0f / _ctx->sampleRate()));
    }
}
