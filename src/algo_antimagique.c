#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/algo_antimagique.h"

/* Fonction utilitaire pour échanger deux pointeurs (arêtes) */
void swap_aretes(aGraphe* g, int i, int j) {
    int* temp = g->matrice_incidence[i];
    g->matrice_incidence[i] = g->matrice_incidence[j];
    g->matrice_incidence[j] = temp;
}

/* 
 * 1. Algorithme Naïf Aléatoire 
 * Fait des échanges aléatoires à chaque itération.
 */
int algo_aleatoire(aGraphe* g, Sommet** sommets, int max_iter, int* nb_swaps) {
    *nb_swaps = 0;
    
    // Mélange initial
    for(int i = 0; i < g->n * 2; i++) {
        swap_aretes(g, rand() % g->n, rand() % g->n);
    }

    for (int iter = 0; iter < max_iter; iter++) {
        calculer_sommes_sommets(g, sommets);
        if (est_antimagique(sommets, g->nb_sommets)) {
            return 1;
        }
        
        // Echange aléatoire simple (random walk)
        int r1 = rand() % g->n;
        int r2 = rand() % g->n;
        swap_aretes(g, r1, r2);
        (*nb_swaps)++;
    }
    
    return 0;
}

/* 
 * 2. Algorithme de Force Brute 
 * Utilisant l'algorithme de Heap pour générer les permutations.
 */
int heap_permute(aGraphe* g, Sommet** sommets, int k, int* nb_swaps) {
    if (k == 1) {
        calculer_sommes_sommets(g, sommets);
        if (est_antimagique(sommets, g->nb_sommets)) {
            return 1;
        }
        return 0;
    }

    for (int i = 0; i < k; i++) {
        if (heap_permute(g, sommets, k - 1, nb_swaps)) return 1;
        
        if (i < k - 1) {
            if (k % 2 == 0) {
                swap_aretes(g, i, k - 1);
            } else {
                swap_aretes(g, 0, k - 1);
            }
            (*nb_swaps)++;
        }
    }
    return 0;
}

int algo_force_brute(aGraphe* g, Sommet** sommets, int* nb_swaps) {
    *nb_swaps = 0;
    if (g->n > 11) { // 12! = 479 millions, commence à être très lent
        printf("[WARN] Force brute ignore car le graphe possede trop d'aretes (n=%d > 11)\n", g->n);
        return 0;
    }
    return heap_permute(g, sommets, g->n, nb_swaps);
}

/*
 * Fonctions utilitaires pour l'heuristique
 */
int compare_ints(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Retourne le nombre de paires de sommets en collision
int compte_collisions(aGraphe* g, Sommet** sommets) {
    calculer_sommes_sommets(g, sommets);
    int cols = 0;
    int* vals = malloc(g->nb_sommets * sizeof(int));
    if (!vals) return 99999;
    
    for(int i = 0; i < g->nb_sommets; i++) {
        vals[i] = sommets[i]->valeur;
    }
    qsort(vals, g->nb_sommets, sizeof(int), compare_ints);
    
    for(int i = 1; i < g->nb_sommets; i++) {
        if(vals[i] == vals[i-1]) cols++;
    }
    free(vals);
    return cols;
}

typedef struct {
    int *pointeur_ligne;
    int degre;
} LigneDegree;

int compare_lignes(const void* a, const void* b) {
    return ((LigneDegree*)a)->degre - ((LigneDegree*)b)->degre;
}

/*
 * 3. Algorithme Heuristique (Tri puis Recuit simulé / recherche locale)
 */
int algo_heuristique(aGraphe* g, Sommet** sommets, int max_iter, int* nb_swaps) {
    *nb_swaps = 0;
    
    // 1. Initialisation : tri hiérarchique par degré d'arête
    LigneDegree* ld = (LigneDegree*)malloc(g->n * sizeof(LigneDegree));
    for(int i = 0; i < g->n; i++) {
        ld[i].pointeur_ligne = g->matrice_incidence[i];
        int d = 0;
        for(int v = 0; v < g->nb_sommets; v++) {
            d += ld[i].pointeur_ligne[v]; // 1 si le sommet v est dans l'arête
        }
        ld[i].degre = d;
    }
    qsort(ld, g->n, sizeof(LigneDegree), compare_lignes); // Ordre croissant
    
    for(int i = 0; i < g->n; i++) {
        g->matrice_incidence[i] = ld[i].pointeur_ligne;
    }
    free(ld);
    
    // 2. Descente de gradient locale
    int current_cols = compte_collisions(g, sommets);
    if(current_cols == 0) return 1;
    
    for(int iter = 0; iter < max_iter; iter++) {
        bool improved = false;
        
        // Explore le 1-voisinage (tous les swaps de 2 aretes possibles)
        for(int i = 0; i < g->n - 1; i++) {
            for(int j = i + 1; j < g->n; j++) {
                swap_aretes(g, i, j);
                (*nb_swaps)++;
                int new_cols = compte_collisions(g, sommets);
                
                if(new_cols < current_cols) {
                    current_cols = new_cols;
                    improved = true;
                    if(current_cols == 0) return 1;
                    break; // On applique le premier swap améliorant trouvé
                }
                
                swap_aretes(g, i, j); // Retour arrière
            }
            if(improved) break;
        }
        
        if(!improved) {
            // Coincé dans un minimum local. On fait un "saut" aléatoire.
            swap_aretes(g, rand() % g->n, rand() % g->n);
            (*nb_swaps)++;
            current_cols = compte_collisions(g, sommets);
            if(current_cols == 0) return 1;
        }
    }
    
    return 0; // Echec
}
