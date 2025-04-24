from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, AveragePooling2D, Flatten, Dense, Dropout

def get_model():
    model = Sequential([
        # Force explicitement une convolution "old-school"
        Conv2D(8, (3, 3), 
               activation='relu', 
               input_shape=(50, 12, 1),
               use_bias=True,  # Désactivez si possible (use_bias=False)
               kernel_initializer='glorot_uniform',
               name='conv2d_v1'),  # Nom explicite

        AveragePooling2D((2, 2)),
        
        Conv2D(16, (3, 3), 
               activation='relu',
               use_bias=True), 
        
        Flatten(),
        Dense(16, activation='relu'),
        Dense(1, activation='hard_sigmoid')
    ])
    return model