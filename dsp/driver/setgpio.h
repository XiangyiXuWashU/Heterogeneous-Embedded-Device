#ifndef _SETGPIO_H_
#define _SETGPIO_H_

#if defined __cplusplus
extern "C" {
#endif

void GPIOInit(void);
void switchLED1(int status);
void switchLED2(int status);
void selectADCChannel(int channel);
void sampleRateControl(int bit3, int bit2, int bit1, int bit0);
void SampleRateChannelSelectGPIOInit(void);

#if defined __cplusplus
}
#endif
#endif
