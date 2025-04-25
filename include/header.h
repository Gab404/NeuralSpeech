#ifndef HEADER_H
#define HEADER_H

#include <arduinoFFT.h>
// #include <Adafruit_SSD1306.h>
#include "globals.h"

#define N_MFCC 12
#define TAPS 57
#define N_SAMPLE 1000
#define WARM_UP_FRAMES 5
#define TOTAL_WINDOW 50
#define OVERLAP 96

#define N_INPUTS 600
#define N_OUTPUTS 1
#define TENSOR_ARENA_SIZE 30 * 1024  // ajuste si besoin

#define PIN_LED_JAUNE 5
#define PIN_LED_BLANC 3
#define PIN_LED_VERT 2
#define PIN_MIC 4

// plink -serial COM5 -sercfg 460800 > "C:\Users\gabri\Downloads\audio.bin"

void setupADC();

float applyRIF(uint16_t buffer[N_SAMPLE], uint16_t indexBuffer);

void getFFT(float window[WIN_SIZE], ArduinoFFT<float> FFT);
void normalizeByEnergy(float window[WIN_SIZE]);

void displayBuffer(volatile uint16_t buffer[N_SAMPLE]);
void writeSample(float value);

void getMFCC(float window[WIN_SIZE], float mfccArray[N_MFCC]);

float applyAGC(float* window, float previousGain);

#endif /*HEADER_H*/