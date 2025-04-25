// Pour entrainer model Python 7.x -> Tensorflow 2.2
#include "header.h"
#include "globals.h"
#include "model.h"
#include "no_macro.h"
#include <EloquentTinyML.h>
#include <algorithm>  

unsigned long time5, time6;

// Globals variables
volatile float window[WIN_SIZE] = {0.0f};
volatile bool windowReady = false;
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

  // Serial.println("---- Démarrage ----");

  pinMode(PIN_MIC, INPUT);
  pinMode(PIN_LED_VERT, OUTPUT);

  pinMode(PIN_LED_BLANC, OUTPUT); // Blanc
  pinMode(PIN_LED_JAUNE, OUTPUT); // Jaune

  // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // display.clearDisplay();

  setupADC();
}

void loop() { // time = 1.3s (37.5% overlap) | time = 1.5s (50% overlap)

  if (!isSignalProcessing && digitalRead(4) == LOW) { // time = 3us
    timeButtonClicked = millis();
    while (millis() - timeButtonClicked <= 1000);
    digitalWrite(PIN_LED_VERT, HIGH);
    isSignalProcessing = true;
  }

  if (windowReady && isSignalProcessing) {
    
    time5 = micros();

    // Copy window to buffer and shift window
    for (uint16_t i = 0; i < WIN_SIZE; i++) { // time = 88 us
      winBuffer[i] = window[i];
      vImag[i] = 0.0f;
      if (i >= WIN_SIZE - OVERLAP)
        window[i - (WIN_SIZE - OVERLAP)] = window[i];
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
    time6 = micros();
    Serial.println(time6 - time5);

    if (++indexMFCC >= TOTAL_WINDOW) {

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

      
      for (uint16_t i = 0; i < WIN_SIZE; i++)
        window[i] = 0.0f;
      
      indexMFCC = 0;
      previousGain = 1.0f;

      // Serial.println("\n-----------------");

      // for (uint16_t i = 0; i < TOTAL_WINDOW; i++) {
      //   for (uint16_t j = 0; j < N_MFCC; j++) {
      //     Serial.print(matrixMFCC[i][j], 6);
      //     if (j < N_MFCC - 1) Serial.print(",");
      //   }
      //   Serial.println();
      // }
      
      isSignalProcessing = false;
      digitalWrite(PIN_LED_VERT, LOW);
    }

    windowReady = false;
  }
}