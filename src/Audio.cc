#include "Audio.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_audio.h"

#include "SynthContext.h"
#include "PulseOscillator.h"
#include "Lowpass.h"

using namespace Synthia;

void initSynth();
void audioCallback(uint16_t *buf , uint16_t length);
uint16_t audioBuffer[BUFF_LEN];

SynthContext synthContext(AUDIO_SAMPLE_RATE);
PulseOscillator osc1;
PulseOscillator lfo1;
Lowpass filt1;

void initAudio()
{
    initSynth();

    BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, AUDIO_SAMPLE_RATE);
    BSP_AUDIO_OUT_SetVolume(80);
    BSP_AUDIO_OUT_Play((uint16_t*)&audioBuffer[0], 2*BUFF_LEN);
}

void initSynth()
{
    osc1.init(&synthContext);
    osc1.setFrequency(440.0f);

    lfo1.init(&synthContext);
    lfo1.setFrequency(2.0f);

    filt1.init(&synthContext);
    filt1.setResonance(0.8f);
    filt1.setCutoff(22000.0f);
}

void audioCallback(uint16_t *buf , uint16_t length)
{
    uint16_t 	pos;
    uint16_t 	*outp;
    float	 	yL, yR ;
    uint16_t 	valueL, valueR;

    outp = buf;

    for (pos = 0; pos < length; pos++)
    {
        //float lfoSamp = lfo1.tick(0);
        //osc1.setFrequency(220.0f + (220.0f * lfoSamp));

        float samp = filt1.tick(0, osc1.tick(0));
        yL = samp;
        yR = samp;

        valueL = (uint16_t)((int16_t)((32767.0f) * yL)); // conversion float -> int
        valueR = (uint16_t)((int16_t)((32767.0f) * yR));

        *outp++ = valueL; // left channel sample
        *outp++ = valueR; // right channel sample
    }
}


/*
    AUDIO DMA CALLBACKS
 */

extern "C" void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
    BSP_LED_Off(LED4);
    audioCallback((uint16_t *)(audioBuffer + BUFF_LEN_DIV2), BUFF_LEN_DIV4);
}

extern "C" void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
    BSP_LED_On(LED4);
    audioCallback((uint16_t *)audioBuffer, BUFF_LEN_DIV4);
}

extern "C" void BSP_AUDIO_OUT_Error_Callback(void)
{
    BSP_LED_On(LED6);
}