import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Nom de votre fichier de données
nom_fichier = "collision.txt"
#nom_fichier = planettes.txt"

# Initialisation des structures pour stocker les coordonnées X et Y de chaque particule
trajectoires = {i: {"x": [], "y": []} for i in range(4)}

# Vérification et lecture du fichier
if not os.path.exists(nom_fichier):
    print(
        f"Erreur : Le fichier '{nom_fichier}' est introuvable. "
        "Assurez-vous qu'il est bien dans le même dossier que ce script."
    )
else:
    # with open(nom_fichier, "r", encoding="utf-8") as fichier:
    #     for ligne in fichier:
    #         ligne = ligne.strip()
    #         if not ligne:
    #             continue
            
    #         # Création du graphique
    #         plt.figure(figsize=(10, 8))

    #         # Séparation par point-virgule (on garde uniquement les 4 premières coordonnées)
    #         elements = ligne.split(";")[:-1]

    #         for i, elem in enumerate(elements):
    #             try:
    #                 # Nettoyage des parenthèses et séparation de X et Y
    #                 coord_str = elem.replace("(", "").replace(")", "")
    #                 x, y = map(float, coord_str.split(","))

    #                 plt.plot(x, y, "o", color="b")
    #             except ValueError:
    #                 # Permet d'ignorer une ligne si le format est corrompu
    #                 continue
    #         # Personnalisation de l'affichage
    #         plt.title("Positions des particules depuis le fichier", fontsize=14, fontweight="bold")
    #         plt.xlabel("Position X", fontsize=12)
    #         plt.ylabel("Position Y", fontsize=12)
    #         plt.grid(True, linestyle="--", alpha=0.6)
    #         plt.axis("equal")  # Échelle 1:1 pour ne pas déformer la géométrie

    #         # Affichage du rendu graphique
    #         plt.show()
    with open(nom_fichier, "r", encoding="utf-8") as fichier:
        # On filtre pour enlever les lignes vides et les retours à la ligne
        lignes = [ligne.strip() for ligne in fichier if ligne.strip()]
    
    fig, ax = plt.subplots(figsize=(8, 4.5))
    
    particules = ax.scatter([], [], c=[], s=1, cmap='jet', vmin=0, vmax=10, edgecolor='none')

    # Configuration fixe des axes (pour éviter que le graphique ne change de taille tout le temps)
    ax.set_title("Positions des particules depuis le fichier", fontsize=14, fontweight="bold")
    ax.set_xlabel("Position X", fontsize=12)
    ax.set_ylabel("Position Y", fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.6)

    # IMPORTANT : Définir des limites fixes pour vos axes (adaptez selon vos données)
    # Exemple : si vos particules bougent entre 0 et 100
    ax.set_xlim(0, 250)
    ax.set_ylim(0, 250)
    ax.set_aspect('equal')  # Équivalent à plt.axis("equal") mais lié à l'axe

    def update(frame):
        # frame correspond à l'index de la ligne actuelle
        ligne = lignes[frame]
        # Séparation par point-virgule (on garde uniquement les 4 premières coordonnées)
        elements = ligne.split(";")[:-1]

        # Listes pour stocker les coordonnées de la frame actuelle
        liste_x = []
        liste_y = []
        liste_v = []

        for elem in elements:
            try:
                # Nettoyage des parenthèses et séparation de X et Y
                coord_str = elem.replace("(", "").replace(")", "").replace(":", ",")
                x, y, v = map(float, coord_str.split(","))
                liste_x.append(x)
                liste_y.append(y)
                liste_v.append(v)
                #print(v, ",")
            except ValueError:
                continue

        # Préparation du format requis pour scatter.set_offsets (colonne X et colonne Y)
        points = np.c_[liste_x, liste_y]
        #print(points)
        particules.set_offsets(points)
        
        # Mise à jour des couleurs en fonction des vitesses collectées
        particules.set_array(np.array(liste_v))
        
        # On retourne l'objet modifié (requis par blit=True)
        return particules,

    ani = animation.FuncAnimation(
        fig=fig, 
        func=update, 
        frames=len(lignes), 
        interval=100, # durée par ligne ms
        blit=False#True
    )

    ani.save('trajectoire_particules.mp4', writer='ffmpeg', fps=10)
    plt.close()
    print("Vidéo générée avec succès !")
