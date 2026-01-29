#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

Graphe* creer_graphe(int n) {
    if (n <= 0) return NULL;
    Graphe* g = (Graphe*)malloc(sizeof(Graphe));
    g->num_sommets = n;
    g->matrice_adj = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g->matrice_adj[i] = (int*)calloc(n, sizeof(int)); // Initialise à 0
    }
    return g;
}

void ajouter_arete(Graphe* g, int u, int v) {
    if (!g || u < 0 || u >= g->num_sommets || v < 0 || v >= g->num_sommets) return;
    // On marque l'arête par 1 (ou autre valeur non nulle) pour dire qu'elle existe structurellement
    g->matrice_adj[u][v] = 1;
    g->matrice_adj[v][u] = 1;
}

void liberer_graphe(Graphe* g) {
    if (!g) return;
    for (int i = 0; i < g->num_sommets; i++) {
        free(g->matrice_adj[i]);
    }
    free(g->matrice_adj);
    free(g);
}

void afficher_graphe(Graphe* g) {
    if (!g) return;
    printf("=== Matrice d'Adjacence (%d sommets) ===\n", g->num_sommets);
    for (int i = 0; i < g->num_sommets; i++) {
        for (int j = 0; j < g->num_sommets; j++) {
            printf("%d ", g->matrice_adj[i][j]);
        }
        printf("\n");
    }
    printf("========================================\n");
}

// Fonction utilitaire pour échanger deux éléments
void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Vérifie si une permutation donne un étiquetage anti-magique
int verifier_configuration(Graphe* g, int* p) {
    // p est un tableau de taille num_sommets contenant une permutation de 1..N
    // Les sommets sont indices 0..N-1. Le label du sommet i est p[i].
    
    // Calculer les poids des arêtes induits
    // On doit stocker les poids et vérifier s'ils sont uniques.
    // Nombre max d'arêtes = N*(N-1)/2.
    int max_edges = g->num_sommets * (g->num_sommets - 1) / 2;
    int* edge_weights = (int*)malloc(max_edges * sizeof(int));
    int count = 0;

    for (int i = 0; i < g->num_sommets; i++) {
        for (int j = i + 1; j < g->num_sommets; j++) {
            if (g->matrice_adj[i][j] != 0) { // Si l'arête existe
                // Poids = somme des sommets incidents
                int w = p[i] + p[j];
                
                // Vérifier si ce poids existe déjà
                for (int k = 0; k < count; k++) {
                    if (edge_weights[k] == w) {
                        free(edge_weights);
                        return 0; // Doublon trouvé, pas anti-magique pour cette config
                    }
                }
                edge_weights[count++] = w;
            }
        }
    }
    
    free(edge_weights);
    return 1; // Tous les poids sont distincts
}

// Fonction récursive pour générer les permutations
void permuter_et_tester(Graphe* g, int* p, int l, int r, int* solution_trouvee) {
    if (*solution_trouvee) return; // Si on a déjà trouvé, on arrête (sauf si on veut TOUTES les solutions)

    if (l == r) {
        if (verifier_configuration(g, p)) {
            printf("Configuration Anti-Magique trouvee : [ ");
            for (int i = 0; i < g->num_sommets; i++) {
                printf("%d ", p[i]);
            }
            printf("]\n");
            *solution_trouvee = 1;
        }
    } else {
        for (int i = l; i <= r; i++) {
            swap((p + l), (p + i));
            permuter_et_tester(g, p, l + 1, r, solution_trouvee);
            swap((p + l), (p + i)); // backtrack
        }
    }
}

void test_antimagique(Graphe* g) {
    if (!g) return;
    printf("Test anti-magique pour graphe de taille %d...\n", g->num_sommets);

    // Initialiser la permutation identité : [1, 2, ..., N]
    int* p = (int*)malloc(g->num_sommets * sizeof(int));
    for (int i = 0; i < g->num_sommets; i++) {
        p[i] = i + 1;
    }

    int solution_trouvee = 0;
    permuter_et_tester(g, p, 0, g->num_sommets - 1, &solution_trouvee);

    if (!solution_trouvee) {
        printf("Aucune configuration anti-magique trouvee.\n");
    } else {
        printf("Le graphe est anti-magique.\n");
    }

    free(p);
}
