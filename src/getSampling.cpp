#include <arduino.h>

void TC0_Handler() {
    // Lit le registre d'état pour effacer le drapeau d'interruption
    TC0->TC_CHANNEL[0].TC_SR;
    // Démarre une nouvelle conversion ADC
    ADC->ADC_CR = ADC_CR_START;
}

void setupADC() {
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; // Active le périphérique ADC
    ADC->ADC_MR = ADC_MR_PRESCAL(0) // Définit le diviseur de fréquence à 255
                  | ADC_MR_STARTUP_SUT64 // Définit le temps de démarrage à 64 périodes d'ADC_CLK
                  | ADC_MR_TRACKTIM(15) // Définit le temps de suivi à 15 périodes d'ADC_CLK
                  | ADC_MR_SETTLING_AST3;// Définit le temps de stabilisation à 17 périodes d'ADC_CLK
  
    ADC->ADC_CHER = 0xC0; // Active le canal 7 (A0)
  
    // Configure Timer Counter 0 Channel 0 (TC0) pour samplingFrequency
    PMC->PMC_PCER0 |= PMC_PCER0_PID27; // Active le périphérique TC0
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_CPCTRG;
    // Définit la source d'horloge à TCLK4 (MCK / 128, 84 MHz / 128 = 656.25 kHz)
    
    // Active le déclenchement de comparaison RC
    // Définit la valeur RC pour une fréquence samplingFrequency Hz
    TC0->TC_CHANNEL[0].TC_RC = 21; // 44Khz -> ( (MCK / 128) / 32000 ) -> 20.51 -> 21
    // Active l'interruption de comparaison RC
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
    // Active l'interruption TC0_IRQn dans le NVIC
    NVIC_EnableIRQ(TC0_IRQn);
    
    // activation du compteur
    // activation du déclenchement logiciel
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
  }