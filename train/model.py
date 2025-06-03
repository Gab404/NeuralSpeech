from tensorflow.keras import layers, models

def get_model(input_shape):
    model = models.Sequential([
        layers.Conv2D(8, (3, 3), activation='relu', input_shape=input_shape),
        layers.MaxPooling2D(pool_size=(2, 2)),

        layers.Conv2D(16, (3, 3), activation='relu'),
        layers.MaxPooling2D(pool_size=(2, 2)),

        layers.Flatten(),
        layers.Dense(16, activation='relu'),
        layers.Dense(4, activation='softmax')  # 4 classes
    ])
    return model
