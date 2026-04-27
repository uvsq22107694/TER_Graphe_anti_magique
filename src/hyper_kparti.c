#include <stdio.h>
#include <stdlib.h>
#include "../include/hyper_kparti.h"

/*
 * Construit l'hypergraphe en utilisant la logique du produit cartésien en base n.
 */
HyperGrapheKParti* construire_hypergraphe_kparti(int k, int n) {
    if (k < 2 || n < 1) {
        return NULL;
    }

    int nb_total_sommets = k * n;
    
    int M = 1;
    for (int i = 0; i < k; i++) {
        M *= n;
    }

    // Allocation de la nouvelle structure englobante
    HyperGrapheKParti* h = (HyperGrapheKParti*)malloc(sizeof(HyperGrapheKParti));
    if (h == NULL) return NULL;
    h->k = k;
    h->n = n;
    h->g = NULL;
    h->sommets = NULL;

    // 1. Allocation des sommets
    Sommet** tous_les_sommets = (Sommet**)calloc(nb_total_sommets, sizeof(Sommet*));
    if (tous_les_sommets == NULL) {
        free(h);
        return NULL;
    }

    for (int i = 0; i < nb_total_sommets; i++) {
        tous_les_sommets[i] = creer_sommet(i + 1);
    }
    h->sommets = tous_les_sommets;

    // 2. Création de aGraphe
    aGraphe* g = creer_graphe(nb_total_sommets, M);
    if (g == NULL) {
        liberer_hypergraphe(h);
        return NULL;
    }
    h->g = g;

    // 3. Tableau temporaire pour les k sommets
    Sommet** arete_courante = (Sommet**)malloc(k * sizeof(Sommet*));
    if (arete_courante == NULL) {
        liberer_hypergraphe(h);
        return NULL;
    }

    // 4. Génération
    for (int i = 0; i < M; i++) {
        int valeur_temp = i; // On prend notre numéro d'arête
        
        // On parcourt de la DERNIÈRE partition (k-1) à la PREMIÈRE (0)
        // Ainsi la dernière partition tourne le plus vite, ce qui correspond
        // exactement à l'ordre lexicographique demandé (A1 B1 C1, puis A1 B1 C2, etc.)
        for (int p = k - 1; p >= 0; p--) {
            // Le "chiffre" actuel nous donne l'index du sommet à choisir (de 0 à n-1)
            int index_dans_partition = valeur_temp % n;
            
            // On décale pour passer au "chiffre" suivant de notre base n
            valeur_temp /= n;
            
            // On récupère le bon sommet. 
            // "p * n" nous amène au début de la partition 'p', 
            // "+ index_dans_partition" sélectionne le bon sommet dedans.
            arete_courante[p] = tous_les_sommets[p * n + index_dans_partition];
        }
        
        if (assigner_aretes_au_label(g, i, k, arete_courante) != 0) {
            free(arete_courante);
            liberer_hypergraphe(h);
            return NULL;
        }
    }

    free(arete_courante);
    return h;
}

/*
 * Fonction de nettoyage dédiée à la nouvelle structure
 */
void liberer_hypergraphe(HyperGrapheKParti* h) {
    if (h != NULL) {
        if (h->g != NULL) {
            // liberer_graphe libère à la fois le aGraphe et les sommets passés en argument
            liberer_graphe(h->g, h->sommets, h->k * h->n);
        } else if (h->sommets != NULL) {
            // Si le graphe a échoué mais les sommets ont été créés
            for (int i = 0; i < h->k * h->n; i++) {
                if (h->sommets[i] != NULL) free(h->sommets[i]);
            }
            free(h->sommets);
        }
        free(h); // Libération finale de la structure conteneur
    }
}
