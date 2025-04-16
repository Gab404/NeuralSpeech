import serial
import struct
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque
import time

# Configuration du port série
SERIAL_PORT = 'COM5'
BAUDRATE = 460800

# Initialisation de la figure
plt.style.use('dark_background')
fig, ax = plt.subplots()
line, = ax.plot([], [], 'lime', lw=1)
ax.set_title("Serial Plotter - Arduino DUE (Binary)", fontsize=12)
ax.set_xlabel("Temps (s)")
ax.set_ylabel("Valeur ADC")
ax.grid(True, linestyle=':', alpha=0.5)

# Buffers pour les données
MAX_DATA_POINTS = 200
x_data = deque(maxlen=MAX_DATA_POINTS)
y_data = deque(maxlen=MAX_DATA_POINTS)

# Connexion série
try:
    ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)
    print(f"Connecté à {SERIAL_PORT} à {BAUDRATE} bauds")
except Exception as e:
    print(f"Erreur de connexion : {e}")
    exit()

# Fonction de mise à jour du graphique
start_time = time.time()

def update(frame):
    while ser.in_waiting >= 2:  # On attend au moins 2 octets
        raw = ser.read(2)
        if len(raw) == 2:
            value = struct.unpack('<H', raw)[0]  # '<H' : uint16_t en little endian
            current_time = time.time() - start_time
            x_data.append(current_time)
            y_data.append(value)
            line.set_data(x_data, y_data)
            ax.relim()
            ax.autoscale_view()

    return line,

# Animation en temps réel
ani = FuncAnimation(fig, update, interval=10, blit=True)
plt.tight_layout()
plt.show()

# Fermeture du port série proprement
ser.close()
print("Fermeture du port série")
