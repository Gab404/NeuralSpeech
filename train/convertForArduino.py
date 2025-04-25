import tensorflow as tf
import sys

model = tf.keras.models.load_model("model.keras")

converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

# === 3. Sauvegarde du modèle .tflite ===
with open("model.tflite", "wb") as f:
    f.write(tflite_model)

print("✅ Modèle TFLite (float32) sauvegardé sous 'model.tflite'")

# === Charger le fichier TFLite ===
with open("model.tflite", "rb") as f:
    tflite_model = f.read()

# === Convertir en tableau C ===
c_array = ', '.join(f'0x{b:02x}' for b in tflite_model)

# === Création du header model.h ===
header = f"""\
#ifndef MODEL_H
#define MODEL_H

const unsigned char model_tflite[] = {{
  {c_array}
}};

const int model_tflite_len = {len(tflite_model)};

#endif  // MODEL_H
"""

# === Sauvegarde ===
with open("model.h", "w") as f:
    f.write(header)

print("✅ Fichier 'model.h' généré avec succès.")