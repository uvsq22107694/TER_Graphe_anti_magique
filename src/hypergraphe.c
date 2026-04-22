#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "../include/hypergraphe.h"

aGraphe* generer_hypergraphe_valide(int nb_sommets, int nb_aretes, Sommet*** tableau_sommets) {
    // Si nous avons plus de sommets que de combinaisons possibles d'arêtes (2^n)
    // l'antimagie est formellement impossible car par le principe des tiroirs de Dirichlet, 
    // au moins 2 sommets auront la même incidence.
    // L'ajout d'une limite à 30 arêtes évite le dépassement d'entier sur 1 << 30.
    if (nb_aretes < 31 && nb_sommets > (1 << nb_aretes)) {
        fprintf(stderr, "Erreur: Impossible de generer %d sommets distincts avec seulement %d aretes.\n", nb_sommets, nb_aretes);
        return NULL;
    }

    *tableau_sommets = (Sommet**)malloc(nb_sommets * sizeof(Sommet*));
    if (*tableau_sommets == NULL) return NULL;
    
    for (int i = 0; i < nb_sommets; i++) {
        (*tableau_sommets)[i] = creer_sommet(i + 1);
    }

    aGraphe* g = creer_graphe(nb_sommets, nb_aretes);
    if (!g) return NULL;

    srand(time(NULL) ^ clock());

    // Pour chaque sommet (colonne), on génère un vecteur d'incidence unique
    for (int v = 0; v < nb_sommets; v++) {
        bool unique = false;
        while (!unique) {
            // Génération de la colonne v
            for (int e = 0; e < nb_aretes; e++) {
                g->matrice_incidence[e][v] = rand() % 2;
            }
            
            // Vérification vis-à-vis des colonnes précédentes
            unique = true;
            for (int prev = 0; prev < v; prev++) {
                bool same = true;
                for (int e = 0; e < nb_aretes; e++) {
                    if (g->matrice_incidence[e][v] != g->matrice_incidence[e][prev]) {
                        same = false;
                        break;
                    }
                }
                if (same) {
                    unique = false;
                    break;
                }
            }
        }
    }

    return g;
}
