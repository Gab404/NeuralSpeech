// Pour entrainer model Python 7.x -> Tensorflow 2.2
#include "header.h"
#include "globals.h"
#include "model.h"
#include "no_macro.h"
#include <EloquentTinyML.h>
#include <algorithm>

// Globals variables
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

// Adafruit_SSD1306 display(128, 64);
ArduinoFFT<float> FFT = ArduinoFFT<float>(winBuffer, vImag, WIN_SIZE, 8000);

Eloquent::TinyML::TfLite<    
    N_INPUTS,
    N_OUTPUTS,
    TENSOR_ARENA_SIZE
> ml;

void setup() {
  Serial.begin(460800);

  ml.begin(model_tflite);

  if (!ml.initialized()) {
    Serial.println("Erreur d'initialisation");
    while (true);
  }

  pinMode(PIN_BTN, INPUT);
  pinMode(PIN_LED_VERT, OUTPUT);

  pinMode(PIN_LED_BLANC, OUTPUT); // Blanc
  pinMode(PIN_LED_JAUNE, OUTPUT); // Jaune

  // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // display.clearDisplay();

  setupADC();
}

void loop() { // time = 1.3s (37.5% overlap) | time = 1.5s (50% overlap)

  if (!isSignalProcessing && digitalRead(PIN_BTN) == LOW) { // time = 3us
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

    // for (uint16_t i = 0; i < WIN_SIZE; i++)
    //   writeSample(winBuffer[i]);
    
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
      digitalWrite(PIN_LED_VERT, LOW);

      uint16_t count = -1;
      for (uint16_t i = 0; i < TOTAL_WINDOW; i++)
        for (uint16_t j = 0; j < N_MFCC; j++) {
          count++;
          vectMFCC[count] = matrixMFCC[i][j];
        }

      float prediction[1];
      if (round(ml.predict(vectMFCC, prediction)) == 0) {
        digitalWrite(PIN_LED_BLANC, HIGH);
        digitalWrite(PIN_LED_JAUNE, LOW);
      } else {
        digitalWrite(PIN_LED_BLANC, LOW);
        digitalWrite(PIN_LED_JAUNE, HIGH);
      }

      
      for (uint16_t i = 0; i < WIN_SIZE; i++) {
        window1[i] = 0.0f;
        window2[i] = 0.0f;
      }
      
      indexMFCC = 0;
      previousGain = 1.0f;
      window_1_processing = true;
      window_2_processing = false;
      window_1_ready = false;
      window_2_ready = false;
      window_1_waiting = false;
      window_2_waiting = false;

      // Serial.println("\n-----------------");

      // for (uint16_t i = 0; i < TOTAL_WINDOW; i++) {
      //   for (uint16_t j = 0; j < N_MFCC; j++) {
      //     Serial.print(matrixMFCC[i][j], 6);
      //     if (j < N_MFCC - 1) Serial.print(",");
      //   }
      //   Serial.println();
      // }
      
      isSignalProcessing = false;
    } else
      windowUpdateStatus();
  }
}