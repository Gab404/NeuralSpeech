import tensorflow as tf
import numpy as np

# 1. Charger votre modèle
model = tf.keras.models.load_model("./model.keras")

# 2. Définir le dataset représentatif (adaptez à votre input_shape (50, 12, 1))
def representative_dataset():
    for _ in range(100):
        # Génère des données simulant vos entrées réelles
        yield [np.random.uniform(low=-1, high=1, size=(1, 50, 12, 1)).astype(np.float32)]

# 3. Configuration du convertisseur
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.representative_dataset = representative_dataset

# 4. Conversion et sauvegarde
tflite_model = converter.convert()
with open("model_quant_int8.tflite", "wb") as f:
    f.write(tflite_model)

print("Conversion INT8 réussie!")

def convert_to_c_array(tflite_path, output_path="model.h"):
    with open(tflite_path, "rb") as f:
        data = f.read()
    with open(output_path, "w") as f:
        f.write("#ifndef MODEL_H\n#define MODEL_H\n\n")
        f.write("const unsigned char model[] = {\n")
        for i, byte in enumerate(data):
            f.write(f"0x{byte:02x}," + ("\n" if (i+1) % 12 == 0 else " "))
        f.write("\n};\n")
        f.write(f"const unsigned int model_len = {len(data)};\n\n")
        f.write("#endif\n")

convert_to_c_array("model_quant_int8.tflite")