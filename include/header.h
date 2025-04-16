#ifndef HEADER_H
    #define HEADER_H

#include <arduinoFFT.h>
#include <Adafruit_SSD1306.h>

#define TAPS 57
#define WIN_SIZE 256
#define N_MFCC 13

// plink -serial COM5 -sercfg 460800 > "C:\Users\gabri\Downloads\audio.bin"

void setupADC();

float applyRIF(u_int16_t buffer[TAPS], uint8_t indexBuffer);

void getFFT(float window[WIN_SIZE], ArduinoFFT<float> FFT);
void normalizeByEnergy(float window[WIN_SIZE]);

void displayBuffer(uint16_t buffer[TAPS]);
void writeSample(u_int16_t sample);

void getMFCC(float window[WIN_SIZE], float mfccArray[N_MFCC]);

#endif /*HEADER_H*/