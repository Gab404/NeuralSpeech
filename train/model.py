from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Flatten, Dense, Dropout

def get_model():
    model = Sequential([
        Conv2D(32, (3, 3), activation='relu', input_shape=(50, 12, 1)),
        MaxPooling2D((2, 2)),
        Conv2D(64, (3, 3), activation='relu'),
        MaxPooling2D((2, 2)),
        Flatten(),
        Dropout(0.3),
        Dense(64, activation='relu'),
        Dense(3, activation='softmax')  # 3 classes
    ])
    return model