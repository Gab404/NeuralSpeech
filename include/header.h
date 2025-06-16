#ifndef HEADER_H
#define HEADER_H

#include <arduinoFFT.h>
#include "globals.h"

// SIGNAL PROCESSING
#define N_MFCC 12
#define TAPS 57
#define N_SAMPLE 1000
#define WARM_UP_FRAMES 5
#define TOTAL_WINDOW 50
#define OVERLAP 96

// IA
#define N_INPUTS 600
#define N_OUTPUTS 4
#define TENSOR_ARENA_SIZE 30 * 1024  // ajuste si besoin

// PIN
#define PIN_LED_JAUNE 5
#define PIN_LED_BLANC 3
#define PIN_LED_VERT 2
#define PIN_BTN 4

// SCREEN
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128

void setupADC();

float applyRIF(uint16_t buffer[N_SAMPLE], uint16_t indexBuffer);

void cpyWinToBuffer(volatile float window[WIN_SIZE], volatile float window_2[WIN_SIZE], float winBuffer[WIN_SIZE], float vImag[WIN_SIZE]);
void windowUpdateStatus();
void writeSample(float value);
void resetGlobals();

void getFFT(float window[WIN_SIZE], ArduinoFFT<float> FFT);
void normalizeByEnergy(float window[WIN_SIZE]);

void getMFCC(float window[WIN_SIZE], float mfccArray[N_MFCC]);

float applyAGC(float* window, float previousGain);

#endif /*HEADER_H*/