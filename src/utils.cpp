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

void writeSample(float value) {
    // Convertir la valeur flottante entre -1 et 1 en un entier 16 bits
    uint16_t sample = (uint16_t)((value + 1.0f) * 16384.0f) - 16384;

    // Récupérer les 8 bits inférieurs
    uint8_t lowByte = sample & 0xFF;
    // Récupérer les 8 bits supérieurs
    uint8_t highByte = (sample >> 8) & 0xFF;

    // Envoyer les deux octets via le port série
    Serial.write(lowByte);
    Serial.write(highByte);
}

void resetGlobals() {
    for (uint16_t i = 0; i < WIN_SIZE; i++) {
        window1[i] = 0.0f;
        window2[i] = 0.0f;
    }

    window_1_processing = true;
    window_2_processing = false;
    window_1_ready = false;
    window_2_ready = false;
    window_1_waiting = false;
    window_2_waiting = false;
}