#include "header.h"

void displayBuffer(volatile uint16_t buffer[N_SAMPLE]) {
    Serial.println("Buffer: ");
    for (int i = 0; i < N_SAMPLE; i++) {
      Serial.print(buffer[i]);
      Serial.print("  ");
    }
    Serial.println("\n_____");
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