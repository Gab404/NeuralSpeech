#include "header.h"
#include "globals.h"
#include "model.h"
#include "no_macro.h"
#include <EloquentTinyML.h>
#include <algorithm>
#include <Wire.h>


// Globals
volatile float window1[WIN_SIZE] = {0.0f};
volatile float window2[WIN_SIZE] = {0.0f};
volatile bool window_1_ready = false;
volatile bool window_2_ready = false;
volatile bool window_1_processing = true;
volatile bool window_2_processing = false;
volatile bool window_1_waiting = false;
volatile bool window_2_waiting = false;
volatile bool isSignalProcessing = false;

unsigned long timeButtonClicked;
float matrixMFCC[TOTAL_WINDOW][N_MFCC];
float vectMFCC[TOTAL_WINDOW * N_MFCC];
uint8_t indexMFCC = 0;
float winBuffer[WIN_SIZE];
float vImag[WIN_SIZE];
float previousGain = 1.0f;
int buttonState = 0;

ArduinoFFT<float> FFT = ArduinoFFT<float>(winBuffer, vImag, WIN_SIZE, 8000);

Eloquent::TinyML::TfLite<
    N_INPUTS,
    N_OUTPUTS,
    TENSOR_ARENA_SIZE
> ml;

void vocalCommand(uint8_t result)
{
  if (result == 0) {
    digitalWrite(PIN_LED_BLANC, HIGH);
    digitalWrite(PIN_LED_JAUNE, LOW);
  } else if (result == 1) {
    digitalWrite(PIN_LED_JAUNE, LOW);
    digitalWrite(PIN_LED_BLANC, LOW);
    Serial.println("Heure");
  } else if (result == 2) {
    digitalWrite(PIN_LED_BLANC, LOW);
    digitalWrite(PIN_LED_JAUNE, HIGH);
  } else {
    Serial.println("Musique");
    digitalWrite(PIN_LED_JAUNE, LOW);
    digitalWrite(PIN_LED_BLANC, LOW);
  }
}

void setup() {
  Serial.begin(460800);

  // IA
  ml.begin(model_tflite);
  if (!ml.initialized()) {
    Serial.println("Erreur TinyML");
    while (true);
  }

  pinMode(PIN_BTN, INPUT);
  pinMode(PIN_LED_VERT, OUTPUT);
  pinMode(PIN_LED_BLANC, OUTPUT);
  pinMode(PIN_LED_JAUNE, OUTPUT);

  setupADC();
}

void loop() {
  if (!isSignalProcessing && digitalRead(PIN_BTN) == LOW) {
    timeButtonClicked = millis();
    while (millis() - timeButtonClicked <= 1000);
    digitalWrite(PIN_LED_VERT, HIGH);
    isSignalProcessing = true;
  }

  if ((window_1_ready || window_2_ready) && isSignalProcessing) {
    if (window_1_ready)
      cpyWinToBuffer(window1, window2, winBuffer, vImag);
    else
      cpyWinToBuffer(window2, window1, winBuffer, vImag);

    previousGain = applyAGC(winBuffer, previousGain);
    getFFT(winBuffer, FFT);
    getMFCC(winBuffer, matrixMFCC[indexMFCC]);

    if (++indexMFCC >= TOTAL_WINDOW) {
      digitalWrite(PIN_LED_VERT, LOW);

      uint16_t count = -1;
      for (uint16_t i = 0; i < TOTAL_WINDOW; i++) {
        for (uint16_t j = 0; j < N_MFCC; j++) {
          count++;
          vectMFCC[count] = matrixMFCC[i][j];
        }
      }

      float prediction[4];
      uint8_t result = 0;
      ml.predict(vectMFCC, prediction);
      for (uint8_t i = 0; i < 4; i++) {
        if (prediction[i] > prediction[result])
          result = i;
      }

      vocalCommand(result);

      indexMFCC = 0;
      previousGain = 1.0f;
      resetGlobals();
      isSignalProcessing = false;
    } else {
      windowUpdateStatus();
    }
  }
}
