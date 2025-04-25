import numpy as np

def generate_dct_matrix(n_mfcc, n_mels):
    dct_matrix = np.zeros((n_mfcc, n_mels))
    factor = np.pi / n_mels

    for i in range(n_mfcc):
        for j in range(n_mels):
            dct_matrix[i, j] = np.cos(i * (j + 0.5) * factor)

    dct_matrix[0, :] *= 1 / np.sqrt(n_mels)
    dct_matrix[1:, :] *= np.sqrt(2 / n_mels)

    return dct_matrix

# Paramètres
n_mfcc = 13
n_mels = 20

# Génération
dct_mat = generate_dct_matrix(n_mfcc, n_mels)

# Écriture dans un fichier
with open("./src/dct_matrix.cpp", "w") as f:
    f.write("#include <avr/pgmspace.h>\n\n")
    f.write(f"const float dct_matrix[{n_mfcc}][{n_mels}] PROGMEM = {{\n")
    for row in dct_mat:
        f.write("  { " + ", ".join(f"{x:.6f}" for x in row) + " },\n")
    f.write("};\n")
