//
// Created by Andy Best on 18/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//

#include "SError.h"

namespace Synthia
{
    string SError::description()
    {
        return _description;
    }

    void SError::setDescription(string description)
    {
        _description = description;
    }
}
