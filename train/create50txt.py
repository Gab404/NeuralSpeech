import os

# 🔧 Spécifie le chemin du dossier où tu veux créer les fichiers
repertoire = "./dataset/test/Blanc"  # ← remplace par ton chemin

# 🛠 Crée le dossier s'il n'existe pas déjà
os.makedirs(repertoire, exist_ok=True)

# 📝 Création des fichiers
for i in range(1, 51):
    chemin_fichier = os.path.join(repertoire, f"{i}.txt")
    open(chemin_fichier, 'w').close()