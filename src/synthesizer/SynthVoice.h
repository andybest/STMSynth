//
//  SynthVoice.h
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#pragma once

#include <stdio.h>

#include "Generator.h"
#include "Triggerable.h"
#include "Controllable.h"

namespace Synthia
{
    class SynthVoice : public PitchedGenerator, public Triggerable, public Controllable
    {

    };
}
