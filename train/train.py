import numpy as np
from tensorflow.keras.utils import to_categorical
from model import get_model
from getData import load_dataset_from_text
import tensorflow as tf

print(tf.__version__)

# Load Data
x_train, y_train, label_map = load_dataset_from_text('./dataset/train')
x_test, y_test, _ = load_dataset_from_text('./dataset/test')

# Standardization
x_test = (x_test - np.mean(x_train)) / np.std(x_train)
x_train = (x_train - np.mean(x_train)) / np.std(x_train)

x_train = x_train[..., np.newaxis]
x_test = x_test[..., np.newaxis]

y_train_cat = to_categorical(y_train, num_classes=3)
y_test_cat = to_categorical(y_test, num_classes=3)

print("X train shape:", x_train.shape)
print("Y train shape:", y_train.shape)
print("X test shape:", x_test.shape)
print("Y test shape:", y_test.shape)

# Neural Network
model = get_model()

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

model.summary()

# Training
model.fit(x_train, y_train, epochs=30, batch_size=16, validation_data=(x_test, y_test))

preds = model.predict(x_test)
pred_classes = np.argmax(preds, axis=1)
model.save("./model.keras")

