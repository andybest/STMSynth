//
//  Triggerable.h
//  TestTool
//
//  Created by Andy Best on 25/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#pragma once

namespace Synthia
{
    class Triggerable
    {
        virtual void keyOn() = 0;
        virtual void keyOff() = 0;
    };
}
