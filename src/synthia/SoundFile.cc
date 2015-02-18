//
// Created by Andy Best on 18/06/2014.
// Copyright (c) 2014 Andy Best. All rights reserved.
//

#include "SoundFile.h"

//#include <fstream>
//#include <iostream>

//#include "SEndian.h"


using namespace std;

namespace Synthia
{
    SoundFile::SoundFile()
    {
        _buffersAllocated = false;
    }

    SoundFile::~SoundFile()
    {
        // Deallocate sample buffers
        if(_buffersAllocated) {
            for(int i=0; i < _channels; i++)
            {
                delete[] _samples[i];
            }

            delete[] _samples;
        }
    }
    
    void SoundFile::loadWav(string path)
    {
#if 0
        ifstream wavFile;

        try
        {
            wavFile.open(path, ios::in );
        } catch(const ifstream::failure& e) {
            throw SoundFileException("File not found");
            return;
        }

        // Get length of file:
        wavFile.seekg (0, wavFile.end);
        int length = (int)wavFile.tellg();
        wavFile.seekg (0, wavFile.beg);
        
        if(length < 1)
            throw SoundFileException("File has zero length");
        
        // Put file into buffer
        unsigned char *fileBuffer = new unsigned char [length];
        wavFile.read((char *)fileBuffer, length);

        wavFile.close();

        cout << "File length: " << length << endl;

        /* RIFF header */
        char id[5];
        unsigned int chunkSize;
        char format[5];
        id[4] = '\0';
        format[4] = '\0';

        char uint16Buf[2];
        char uint32Buf[4];
        
        memcpy(id, fileBuffer, sizeof(char) * 4);
        memcpy(format, &fileBuffer[8], sizeof(char) * 4);
        
        if(strcmp(id, "RIFF") || strcmp(format, "WAVE"))
        {
            delete[] fileBuffer;
            throw SoundFileException("Invalid file format");
        }
        
        //
        memcpy(uint32Buf, &fileBuffer[4], sizeof(char) * 4);
        chunkSize = le32toh(*((uint32_t*)uint32Buf));
        
        /* fmt Chunk */
        
        // Get the size of the format chunk
        unsigned int subChunk1Size;
        memcpy(uint32Buf, &fileBuffer[16], sizeof(char) * 4);
        subChunk1Size = le32toh(*((uint32_t*)uint32Buf));
        
        // Get the format of the audio (PCM is 1)
        unsigned int audioFormat;   // 16 bit value
        memcpy(uint16Buf, &fileBuffer[20], sizeof(char) * 2);
        audioFormat = le16toh(*((uint16_t*)uint16Buf));
        
        if(audioFormat != 1)
        {
            delete[] fileBuffer;
            throw SoundFileException("This decoder only supports PCM encoded WAV files.");
        }
        
        // Get number of channels
        unsigned int numChannels;   // 16 bit value
        memcpy(uint16Buf, &fileBuffer[22], sizeof(char) * 2);
        numChannels = le16toh(*((uint16_t*)uint16Buf));
        
        // Get the sample rate
        unsigned int sampleRate;
        memcpy(uint32Buf, &fileBuffer[24], sizeof(char) * 4);
        sampleRate = le32toh(*((uint32_t*)uint32Buf));
        
        // Bit depth of the wav file
        unsigned int bitDepth;
        memcpy(uint16Buf, &fileBuffer[16], sizeof(char) * 2);
        bitDepth = le16toh(*((uint16_t*)uint16Buf));

        // Start of the data chunk.
        int dataStart = 20 + subChunk1Size + 4;

        // Size of the sample data
        unsigned int subchunk2Size;
        memcpy(uint32Buf, &fileBuffer[dataStart], sizeof(char) * 4);
        subchunk2Size = le32toh(*((uint32_t*)uint32Buf));

        int numSamples = (subchunk2Size / numChannels) / (bitDepth / 8);

        _channels = numChannels;
        _sampleRate = sampleRate;
        _length = numSamples;

        // Allocate sample buffers
        _samples = new float* [numChannels];
        for(int i=0; i < numChannels; i++)
        {
            _samples[i] = new float[numSamples];
        }

        unsigned char *wavSamples = fileBuffer + dataStart + 4;

        for(int i=0; i < numSamples; i++)
        {
            int sampIdx = i * numChannels * (bitDepth / 8);

            if(bitDepth == 16) {
                for(int c=0; c < numChannels; c++) {
                    unsigned char *sPtr = wavSamples + sampIdx + (c * (bitDepth / 8));
                    int16_t samp = le16toh(*((uint16_t*)sPtr));
                    float halfMax = UINT16_MAX / 2;

                    _samples[c][i] = (samp - halfMax) / halfMax;;
                }
            }
        }

        delete[] fileBuffer;
#endif
    }
        
}
