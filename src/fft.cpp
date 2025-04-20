#include "header.h"

void normalizeByEnergy(float window[WIN_SIZE]) {
    float energy = 0.0f;
  
    for (uint16_t i = 0; i < WIN_SIZE; i++)
      energy += window[i] * window[i];
  
    energy = sqrt(energy);
    if (energy <= 0.000001)
      energy = 0.00001;
    
    for (uint16_t i = 0; i < WIN_SIZE; i++)
      window[i] = window[i] / energy;
}

void getFFT(float window[WIN_SIZE], ArduinoFFT<float> FFT) {

    FFT.dcRemoval(window, WIN_SIZE); // time = 770 us
    FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD); // time = 7 050 us
    FFT.compute(FFT_FORWARD); // time = 13 170 us
    FFT.complexToMagnitude(); // time = 1 565 us
}

