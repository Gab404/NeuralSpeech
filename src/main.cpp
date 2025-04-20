#include <Arduino.h>
#include "header.h"
#include "globals.h"

unsigned long time0, time2;

// Globals variables
volatile float window[WIN_SIZE] = {0.0f};
volatile bool windowReady = false;
volatile bool isSignalProcessing = false;

unsigned long timeButtonClicked, timeToStart;

float matrixMFCC[TOTAL_WINDOW][N_MFCC];
uint8_t indexMFCC = 0;

float winBuffer[WIN_SIZE];
float vImag[WIN_SIZE];

float previousGain = 1.0f;

int buttonState = 0;

Adafruit_SSD1306 display(128, 64);
ArduinoFFT<float> FFT = ArduinoFFT<float>(winBuffer, vImag, WIN_SIZE, 8000);

void setup() {
  Serial.begin(460800);

  // Serial.println("---- Démarrage ----");

  pinMode(4, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  setupADC();
}

void loop() { // time = 2.5s for 1s 

  if (!isSignalProcessing && digitalRead(4) == LOW) { // time = 3us
    timeButtonClicked = millis();
    while (millis() - timeButtonClicked <= 1000);
    isSignalProcessing = true;
    time0 = micros();

    // Serial.println("Start");
  }

  if (windowReady && isSignalProcessing) {

    // Copy window to buffer and shift window
    for (uint16_t i = 0; i < WIN_SIZE / 2; i++) { // time = 55 us
      winBuffer[i] = window[i];
      winBuffer[i + (WIN_SIZE / 2)] = window[i + (WIN_SIZE / 2)];
      window[i] = window[i + (WIN_SIZE / 2)];
      vImag[i] = 0.0f;
      vImag[i + (WIN_SIZE / 2)] = 0.0f;
    }

    previousGain = applyAGC(winBuffer, previousGain); // time = 770 us

    getFFT(winBuffer, FFT); // time = 22 380 us


    // ---- Display OLED ----

    // uint8_t bar_height;

    // display.clearDisplay();
    // for (uint8_t i = 0; i < WIN_SIZE / 2; i++) {
    //     bar_height = (uint8_t)((winBuffer[i] / 13) * 64);
    //     display.drawLine(i, 63, i, 63 - bar_height, WHITE);
    // }
  
    // display.display();

    /*------------------------*/

    getMFCC(winBuffer, matrixMFCC[indexMFCC]); // time = 3 350 us


    if (++indexMFCC >= TOTAL_WINDOW) {
      time2 = micros();
      for (uint16_t i = 0; i < WIN_SIZE; i++)
        window[i] = 0.0f;
      
      indexMFCC = 0;
      previousGain = 1.0f;

      // Serial.println("\n-----------------");

      // for (uint16_t i = 0; i < TOTAL_WINDOW; i++) {
      //   for (uint16_t j = 0; j < N_MFCC; j++) {
      //     Serial.print(matrixMFCC[i][j]);
      //     if (j < N_MFCC - 1) Serial.print(",");
      //   }
      //   Serial.println();
      // }
      Serial.println(time2 - time0);
      isSignalProcessing = false;
    }

    windowReady = false;
  }
}