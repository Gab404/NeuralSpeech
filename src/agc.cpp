#include "header.h"

#define TARGET_RMS 0.2f
#define EPSILON 1e-6f
#define SMOOTHING 0.9f

float applyAGC(float* window, float previousGain) {
    float sum_squares = 0.0f;
    float rms, gain;

    for (int i = 0; i < WIN_SIZE; i++)
        sum_squares += window[i] * window[i];

    rms = sqrtf(sum_squares / WIN_SIZE);
    if (rms <= 0.1) // To avoid increasing the noise (rms < 0.1)
        return 1.0f;

    gain = TARGET_RMS / (rms + EPSILON);

    gain = SMOOTHING * previousGain + (1.0f - SMOOTHING) * gain;

    for (int i = 0; i < WIN_SIZE; i++)
        window[i] *= gain;

    return gain;
}
