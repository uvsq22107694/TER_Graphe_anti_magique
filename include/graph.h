#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct Graphe {
    int num_sommets;
    int** matrice_adj; // Matrice d'adjacence : matrice_adj[i][j] = poids de l'arête (0 si pas d'arête)
} Graphe;

// Créer un graphe avec n sommets
Graphe* creer_graphe(int n);

// Créer un graphe biparti complet K_{m,n}
Graphe* creer_graphe_biparti_complet(int m, int n);

// Ajouter une arête entre u et v (non orienté). 
// Le poids n'est pas spécifié ici car "poids par défaut = 0", l'existence est marquée par la symétrie ?
// Si on veut marquer l'existence, on peut mettre 1. Mais l'utilisateur a dit "poids par défaut = 0".
// Si l'arête existe avec poids 0, il faut un moyen de le savoir.
// On va supposer que 0 = pas d'arête. Si on veut une arête de poids 0, c'est ambigu.
// Pour l'instant on mettra 1 pour dire "existe" lors de la construction structurelle.
void ajouter_arete(Graphe* g, int u, int v);

// Libérer le graphe
void liberer_graphe(Graphe* g);

// Affiche le graphe
void afficher_graphe(Graphe* g);

// Teste si le graphe est anti-magique
// "en testant toutes les configuration de sommet dans ce graphe et en donnant comme poids des arretes la sommet des sommets incident à l'arrete"
void test_antimagique(Graphe* g);

#endif
