#include <Arduino.h>
#include "header.h"

const int16_t coeff[TAPS] = {
    -310, -377, -451, -386, -148, 229,
    650, 975, 1080, 909, 511, 37, -317,
    -388, -127, 366, 863, 1102, 902, 266,
    -590, -1283, -1403, -673, 918, 3077,
    5273, 6907, 7511, 6907, 5273, 3077, 918,
    -673, -1403, -1283, -590, 266, 902, 1102,
    863, 366, -127, -388, -317, 37, 511, 909,
    1080, 975, 650, 229, -148, -386, -451, -377, -310};


float applyRIF(uint16_t buffer[N_SAMPLE], uint16_t indexBuffer) {
    int64_t result = 0;
    // uint16_t finalResult;
    uint8_t currIndexBuffer = indexBuffer;
    // int32_t tmp;

    for (int i = 0; i < TAPS; i++) {
        result += coeff[i] * buffer[currIndexBuffer];
        currIndexBuffer = (currIndexBuffer - 1 + TAPS) % TAPS;
    }

    result -= 70000000; // Normalisation entre -1 et 1 avec un max de 140 000 000

    return ((float)result) / 70000000.0f;
}