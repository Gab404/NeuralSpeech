#include <arduino.h>
#include "header.h"
#include "globals.h"

#define FS_ORIGINAL 32000
#define FS_TARGET 8000
#define DECIMATION_FACTOR (FS_ORIGINAL/FS_TARGET)

bool bufferReady = false;
uint16_t samplesCollected = 0;
uint8_t downsampleCounter = 0;

uint16_t buffer[N_SAMPLE] = {0};
uint16_t indexBuffer = 0;
uint16_t indexBufferToWin;

uint16_t indexWindow = 0;

void TC0_Handler() {
    // Lit le registre d'état pour effacer le drapeau d'interruption
    volatile uint32_t status = TC0->TC_CHANNEL[0].TC_SR;
    (void)status; // Évite un warning "unused variable"
    // Démarre une nouvelle conversion ADC
    ADC->ADC_CR = ADC_CR_START;
}

// Modification de ADC_Handler
void ADC_Handler() {
    if (ADC->ADC_ISR & ADC_IER_EOC7) {
        uint16_t value = ADC->ADC_CDR[7];

        __disable_irq();
        bool local_isSignalProcessing = isSignalProcessing;
        __enable_irq();

        if (local_isSignalProcessing) { // time = 26us 
            buffer[indexBuffer] = value;

            // On attend de stocker les 57 (TAPS) premières valeurs dans le buffer
            if (bufferReady == false) {
                if (++samplesCollected >= TAPS) {
                    bufferReady = true;
                }
                indexBuffer = (indexBuffer + 1) % N_SAMPLE;
                indexBufferToWin = indexBuffer;
                return;
            }

            // Filtrage et décimation
            if (++downsampleCounter >= DECIMATION_FACTOR && ((window_1_processing && !window_1_ready) || (window_2_processing && !window_2_ready))) {
                downsampleCounter = 0;
                if (window_1_processing)
                    window1[indexWindow + OVERLAP] = applyRIF(buffer, indexBufferToWin); // time = 25us
                else
                    window2[indexWindow + OVERLAP] = applyRIF(buffer, indexBufferToWin); // time = 25us
                // writeSample(window[indexWindow + OVERLAP]);

                if (++indexWindow >= WIN_SIZE - OVERLAP) {
                    if (window_1_processing) {
                        window_1_processing = false;
                        if (window_2_ready)
                            window_1_waiting = true;
                        else {
                            window_1_ready = true;
                            window_2_processing = true;
                        }
                    } else {
                        window_2_processing = false;
                        if (window_1_ready)
                            window_2_waiting = true;
                        else {
                            window_2_ready = true;
                            window_1_processing = true;
                        }
                    }
                    indexWindow = 0;
                }
            }
            
            if (((window_1_processing && !window_1_ready) || (window_2_processing && !window_2_ready)))
                indexBufferToWin = (indexBufferToWin + 1) % N_SAMPLE;

            indexBuffer = (indexBuffer + 1) % N_SAMPLE;
        } else {
            __disable_irq();
            indexBuffer = 0;
            indexWindow = 0;
            bufferReady = false;
            samplesCollected = 0;
            downsampleCounter = 0;
            __enable_irq();
        }
    }
}

void setupADC() {
    // 1. Configuration ADC plus précise
    PMC->PMC_PCER1 |= PMC_PCER1_PID37;
    ADC->ADC_MR = ADC_MR_PRESCAL(1)          // Meilleure précision
                | ADC_MR_STARTUP_SUT64
                | ADC_MR_TRACKTIM(15)
                | ADC_MR_SETTLING_AST17;     // Plus long temps de stabilisation
    
    ADC->ADC_CHER = ADC_CHER_CH7;            // Canal 7 uniquement

    // 2. Configuration timer exacte pour 32kHz
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;
    // uint32_t rc = (VARIANT_MCK / 128) / FS_ORIGINAL; // 84MHz/128/32000 = 20.507
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_CPCTRG;
    TC0->TC_CHANNEL[0].TC_RC = 21; // 20 ou 21 selon besoin
    
    // 3. Activation interruptions
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
    NVIC_EnableIRQ(TC0_IRQn);
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    
    ADC->ADC_IER = ADC_IER_EOC7;
    NVIC_EnableIRQ(ADC_IRQn);
}