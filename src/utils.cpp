#include "header.h"

void cpyWinToBuffer(volatile float window[WIN_SIZE], volatile float window_2[WIN_SIZE], float winBuffer[WIN_SIZE], float vImag[WIN_SIZE]) {
    for (uint16_t i = 0; i < WIN_SIZE; i++) { // time = 88 us
        winBuffer[i] = window[i];
        vImag[i] = 0.0f;
        if (i >= WIN_SIZE - OVERLAP)
            window_2[i - (WIN_SIZE - OVERLAP)] = window[i];
    }
}

void windowUpdateStatus() {
    if (window_1_ready) {
        window_1_ready = false;
        if (window_2_waiting) {
            window_2_ready = true;
            window_2_waiting = false;
            window_1_processing = true;
        }
    } else if (window_2_ready) {
        window_2_ready = false;
        if (window_1_waiting) {
            window_1_ready = true;
            window_1_waiting = false;
            window_2_processing = true;
        }
    }
}