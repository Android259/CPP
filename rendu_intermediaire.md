# Rendu intermédiaire

## TP 1

### Euler explicite

Pour un pas 1e-1 : erreur 0.269405
Pour un pas 1e-2 : erreur 0.0428442
Pour un pas 1e-3 : erreur 0.00450504
Pour un pas 1e-4 : erreur 0.000452792
Pour un pas 1e-5 : erreur 4.53021e-05

Sur un diagramme log-log, on observe une droite croissante de pente 1.

### Euler explicite-implicite

Pour un pas 2e-1 : implicite : -0.207398, explicite : 3.88041e+08
Pour un pas 2e-2 : implicite : -0.379307, explicite : -0.379452
Pour un pas 2e-3 : implicite : -0.395959, explicite : -0.395974

On observe que pour un pas grand, la méthode explicite a divergé tandis que la méthode implicite est restée proche de la solution.

## TP 2

### Question 3

Sortie du programme `benchmarks/lab2_collections` (comparaison des quatre conteneurs, parcours répété 1000 fois) :

```
Comparaison des conteneurs (parcours repete 1000 fois)

n = 64 particules
  vector   insertion     9.501 us   parcours    49.044 us   (somme = 65277.6)
  deque    insertion     2.795 us   parcours    62.039 us   (somme = 65277.6)
  list     insertion     5.799 us   parcours    83.905 us   (somme = 65277.6)
  set      insertion    13.484 us   parcours   181.923 us   (somme = 65277.6)

n = 128 particules
  vector   insertion     8.943 us   parcours    96.341 us   (somme = 124793.0)
  deque    insertion     2.794 us   parcours    97.040 us   (somme = 124793.0)
  list     insertion     7.266 us   parcours   165.296 us   (somme = 124793.0)
  set      insertion    13.624 us   parcours   406.114 us   (somme = 124793.0)

n = 256 particules
  vector   insertion    19.491 us   parcours   195.895 us   (somme = 251706.3)
  deque    insertion     3.843 us   parcours   201.834 us   (somme = 251706.3)
  list     insertion    10.480 us   parcours   513.493 us   (somme = 251706.3)
  set      insertion    21.308 us   parcours   943.569 us   (somme = 251706.3)

n = 512 particules
  vector   insertion    39.543 us   parcours   389.905 us   (somme = 507463.9)
  deque    insertion     6.707 us   parcours   381.732 us   (somme = 507463.9)
  list     insertion    19.073 us   parcours   989.609 us   (somme = 507463.9)
  set      insertion    51.140 us   parcours  1895.942 us   (somme = 507463.9)

n = 1024 particules
  vector   insertion    82.299 us   parcours   765.838 us   (somme = 1006031.7)
  deque    insertion    11.248 us   parcours   779.881 us   (somme = 1006031.7)
  list     insertion    36.050 us   parcours  1911.870 us   (somme = 1006031.7)
  set      insertion    67.209 us   parcours  4020.196 us   (somme = 1006031.7)

n = 2048 particules
  vector   insertion   118.068 us   parcours  1566.678 us   (somme = 2029741.1)
  deque    insertion    19.981 us   parcours  1597.277 us   (somme = 2029741.1)
  list     insertion    71.190 us   parcours  4486.741 us   (somme = 2029741.1)
  set      insertion   158.938 us   parcours  7930.008 us   (somme = 2029741.1)

n = 4096 particules
  vector   insertion   295.311 us   parcours  3114.631 us   (somme = 4075345.1)
  deque    insertion    40.660 us   parcours  3558.960 us   (somme = 4075345.1)
  list     insertion   163.270 us   parcours  8473.053 us   (somme = 4075345.1)
  set      insertion   335.342 us   parcours 19654.136 us   (somme = 4075345.1)

n = 8192 particules
  vector   insertion   520.339 us   parcours  6250.432 us   (somme = 8184258.4)
  deque    insertion   146.573 us   parcours  7621.424 us   (somme = 8184258.4)
  list     insertion   452.223 us   parcours 18627.011 us   (somme = 8184258.4)
  set      insertion   836.051 us   parcours 47006.615 us   (somme = 8184258.4)
```

À l'insertion, `deque` gagne car `vector` doit périodiquement réallouer et recopier son tableau quand il grandit. Au parcours, `vector` gagne grâce à sa mémoire contiguë ; `list` et `set` sautent de nœud en nœud (non contigus en mémoire) et deviennent 3 à 7 fois plus lents que `vector`/`deque`. L'écart devient net à partir de 1000 particules environ. Pour une simulation qui parcourt l'ensemble des particules à chaque pas de temps, `vector` est donc le conteneur le plus adapté.

### Question 5

Pour vérifier que l'implémentation est correcte, on peut déjà afficher les trajectoires pour vérifier qu'elles correspondent à ce qui est attendu. Mais on peut aussi calculer l'énergie mécanique du système pour vérifier qu'elle ne varie pas dans le temps.

## TP 3

### Question 7

Temps d'insertion dans le conteneur, pour un univers de (2^k)^3 particules réparties sur une grille régulière du cube unitaire :

k=3, n=512 particules : insertion 0.0001 s
k=4, n=4096 particules : insertion 0.0004 s
k=5, n=32768 particules : insertion 0.0023 s
k=6, n=262144 particules : insertion 0.0203 s
k=7, n=2097152 particules : insertion 0.1557 s

### Question 8

Temps de calcul des interactions (toutes les paires, O(n^2)), pour le même univers :

k=3, n=512 particules : temps 0.0007 s, nombre de paires 1.308e+05
k=4, n=4096 particules : temps 0.0448 s, nombre de paires 8.387e+06
k=5, n=32768 particules : temps 3.0533 s, nombre de paires 5.369e+08
k=6, n=262144 particules : temps ~195.4 s (extrapolé en O(n^2))
k=7, n=2097152 particules : temps ~12506.9 s, soit ~208 min (extrapolé en O(n^2))

Les temps de calcul des interactions sont très longs par rapport à la création, car il faut itérer sur tous les couples de particules. Le coût croit en O(n^2) : un facteur ~8 sur n (de k à k+1) donne un facteur ~64 sur le nombre de paires et donc sur le temps, ce qui est confirmé par les mesures (passage de k=4 à k=5 : 3.0533/0.0448 = 68). Au-delà de k=5, le calcul devient trop long pour être mesuré directement et est donc extrapolé.

### Question 9

Une modification simple est de ne considérer chaque couple qu'une seule fois (en parcourant les paires $i<j$) et de changer les forces sur les deux particules à la fois, puisque $F_{ji} = -F_{ij}$. Cela divise par deux le nombre de paires traitées, même si chaque paire coûte un peu plus cher (deux mises à jour de force au lieu d'une).

### Question 10

Autres simplifications possibles :
- Ignorer les paires de particules séparées par une distance supérieure au rayon de coupure, ce qui permet de ne considérer que les particules voisines (découpage en cellules, voir TP4) et de faire passer le coût de $O(n^2)$ à $O(n)$.
- Mettre en cache les quantités qui ne changent pas à chaque pas (par exemple $1/masse$, $\sigma^2$, le carré du rayon de coupure, la taille des cellules).
- Éviter les allocations dans la boucle en temps (réutiliser un tampon de forces, utiliser des tableaux de taille fixe pour les coordonnées de cellule).
- Mettre à jour les cellules de manière incrémentale (une particule qui change de cellule) et ne pas tout reconstruire à chaque pas.

## TP 4

### Question 1

Le diagramme montre un minimum en $r_{eq}=2^{1/6}\sigma$ : en dessous, les particules se repoussent fortement, au-dessus elles s'attirent faiblement. Dans la formule, cela correspond au terme $(\sigma/r)^{12}$ qui domine à courte distance et au terme $(\sigma/r)^6$ qui domine à longue distance.

Cette force est plus locale que la gravitation : elle diminue en $1/r^7$ contre $1/r^2$ pour la gravité, et contrairement à celle-ci (toujours attractive), elle s'inverse selon la distance.

### Question 2

Une manière est de mettre les particules dans des cases en fonction de leur position et l'univers dispose des cellules et de leurs position.
