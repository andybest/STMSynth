//
// Created by Andy Best on 18/02/2015.
// Copyright (c) 2014 Andy Best. All rights reserved.
//


#include "SObject.h"
#include "SynthContext.h"

#ifndef __Effect_H_
#define __Effect_H_

namespace Synthia
{
    class Effect : public SObject
    {
        virtual void init(SynthContext *ctx) = 0;
        virtual float tick(int channel, float input) = 0;

    protected:
        SynthContext *_ctx;
    };
}


#endif //__Effect_H_
