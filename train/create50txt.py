import os

# 🔧 Spécifie le chemin du dossier où tu veux créer les fichiers
repertoire = "./Musique"  # ← remplace par ton chemin

# 🛠 Crée le dossier s'il n'existe pas déjà
os.makedirs(repertoire, exist_ok=True)

# 📝 Création des fichiers
for i in range(1, 5):
    chemin_fichier = os.path.join(repertoire, f"{i+5}.txt")
    open(chemin_fichier, 'w').close()