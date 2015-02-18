//
// Created by Andy Best on 18/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//


#ifndef __SoundFile_H_
#define __SoundFile_H_

#include "SError.h"

#include <string>
#include <exception>

using std::string;

namespace Synthia
{
    struct SoundFileException
    {
        std::string s;
        SoundFileException(std::string ss) : s(ss) {}
        ~SoundFileException() throw () {}
        const char* what() const throw() { return s.c_str(); }
    };

    class SoundFile
    {
    public:
        SoundFile();
        ~SoundFile();
        void loadWav(string path);
        inline float **samples() { return _samples; }
        inline int length() { return _length; }
        inline int channels() { return _channels; }
        
    private:
        float **_samples;
        int _length;
        int _channels;
        int _sampleRate;
        bool _buffersAllocated;

    };
}

#endif //__SoundFile_H_
