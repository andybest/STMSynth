#pragma once


#define BUFF_LEN_DIV4           512                 // Num samples per channel
#define BUFF_LEN_DIV2           (2*BUFF_LEN_DIV4)
#define BUFF_LEN                (4*BUFF_LEN_DIV4)   // Audio buffer length

#define AUDIO_SAMPLE_RATE       48000

void initAudio();