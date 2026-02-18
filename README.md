# TER_Graphe_anti_magique
Projet de Master TER sur les graphes anti magique

## Algorithmes

### Algorithme glouton décroissant

Cet algorithme est une heuristique qui tente de trouver un étiquetage anti-magique sur les arêtes d'un graphe. Il fonctionne de la manière suivante :

1. On commence avec le poids le plus élevé possible, qui est le nombre d'arêtes du graphe.
2. On choisit un sommet v qui maximise le score suivant : (somme des poids des arêtes incidentes déjà pondérées) + (k * nombre d'arêtes incidentes non pondérées). Avec k le poids le plus élevé restant.
3. On attribue les poids k, k-1, ... aux arêtes incidentes non pondérées de v
4. On répète les étapes 2 et 3 jusqu'à ce que toutes les arêtes aient été pondérées

Cet algorithme n'est pas garanti de trouver un étiquetage anti-magique.

### Algorithme par degré croissant

Cet algorithme est une heuristique qui tente de trouver un étiquetage anti-magique sur les arêtes d'un graphe. Il fonctionne de la manière suivante :

1. On commence avec le poids le plus faible possible, qui est 1.
2. On trie les sommets par degré croissant.
3. On prend le premier sommet v de la liste.
4. On attribue les poids k, k+1, k+2, ... aux arêtes incidentes non pondérées de v. Avec k le poids le plus faible restant.
5. On répète les étapes 3 et 4 jusqu'à ce que toutes les arêtes aient été pondérées.

Cet algorithme n'est pas garanti de trouver un étiquetage anti-magique.