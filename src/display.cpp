#include "header.h"

void displayBuffer(uint16_t buffer[TAPS]) {
    Serial.println("Buffer: ");
    for (int i = 0; i < TAPS; i++) {
      Serial.print(buffer[i]);
      Serial.print("  ");
    }
    Serial.println("\n_____");
  }
  
  void writeSample(u_int16_t sample) {
    uint8_t lowByte = sample & 0xFF; // Récupérer les 8 bits inférieurs
    uint8_t highByte = (sample >> 8) & 0xFF; // Récupérer les 8 bits supérieurs
  
    // Envoyer les deux octets via le port série
    Serial.write(lowByte);
    Serial.write(highByte);
  }