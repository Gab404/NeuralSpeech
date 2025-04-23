import os
import numpy as np

def load_dataset_from_text(dataset_dir, expected_shape=(50, 13)):
    X = []
    Y = []
    label_map = {}

    for i, label_name in enumerate(sorted(os.listdir(dataset_dir))):
        label_path = os.path.join(dataset_dir, label_name)
        if not os.path.isdir(label_path):
            continue

        label_map[label_name] = i

        for file_name in os.listdir(label_path):
            file_path = os.path.join(label_path, file_name)

            if file_name.endswith('.txt') or file_name.endswith('.csv'):
                try:
                    mfcc = np.loadtxt(file_path, delimiter=",")
                    if mfcc.shape == expected_shape:
                        X.append(mfcc)
                        Y.append(i)
                    else:
                        print(f"⚠️ Mauvaise forme dans {file_path}: {mfcc.shape} (attendu {expected_shape})")
                except Exception as e:
                    print(f"❌ Erreur lors du chargement de {file_path}: {e}")

    X = np.array(X)
    Y = np.array(Y)

    return X, Y, label_map