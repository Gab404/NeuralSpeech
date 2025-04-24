import tensorflow as tf

interpreter = tf.lite.Interpreter(model_path="model_quant.tflite")
interpreter.allocate_tensors()

for detail in interpreter.get_tensor_details():
    print(detail)
