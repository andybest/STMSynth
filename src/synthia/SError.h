//
// Created by Andy Best on 18/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//


#ifndef __SError_H_
#define __SError_H_

#include <string>

using std::string;

namespace Synthia
{
    class SError
    {
    public:
        string description();
        void setDescription(string description);
    private:
        string _description;
    };
}

#endif //__SError_H_
