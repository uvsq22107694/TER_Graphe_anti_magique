#include <stdio.h>
#include <stdlib.h>
#include "../include/hyper_kparti.h"

HyperGrapheKParti* construire_hypergraphe_biparti(int n) {
    if (n < 1) return NULL;

    int k = 2;
    int M = n * n;
    int nb_total_sommets = k * n;

    HyperGrapheKParti* h = (HyperGrapheKParti*)malloc(sizeof(HyperGrapheKParti));
    if (h == NULL) return NULL;
    h->k = k;
    h->n = n;

    h->sommets = (Sommet**)calloc(nb_total_sommets, sizeof(Sommet*));
    if (h->sommets == NULL) {
        free(h);
        return NULL;
    }

    for (int i = 0; i < nb_total_sommets; i++) {
        h->sommets[i] = creer_sommet(i + 1);
    }

    h->g = creer_graphe(nb_total_sommets, M);
    if (h->g == NULL) {
        liberer_hypergraphe(h);
        return NULL;
    }

    Sommet** arete_courante = (Sommet**)malloc(k * sizeof(Sommet*));
    if (arete_courante == NULL) {
        liberer_hypergraphe(h);
        return NULL;
    }

    // Création des arêtes dans l'ordre lexicographique ({A1, B1}=1, {A1, B2}=2, etc.)
    for (int i = 0; i < M; i++) {
        int a_idx = i / n;
        int b_idx = i % n;
        arete_courante[0] = h->sommets[a_idx];
        arete_courante[1] = h->sommets[n + b_idx];
        
        if (assigner_aretes_au_label(h->g, i, k, arete_courante) != 0) {
            free(arete_courante);
            liberer_hypergraphe(h);
            return NULL;
        }
    }

    free(arete_courante);
    return h;
}

HyperGrapheKParti* ajouter_partition_hypergraphe(HyperGrapheKParti* h) {
    if (h == NULL || h->g == NULL) return NULL;

    int old_k = h->k;
    int n = h->n;
    int new_k = old_k + 1;
    
    int old_M = h->g->n; // Le nombre d'arêtes actuel
    int new_M = old_M * n;
    
    int new_nb_sommets = new_k * n;

    HyperGrapheKParti* new_h = (HyperGrapheKParti*)malloc(sizeof(HyperGrapheKParti));
    if (new_h == NULL) return NULL;
    new_h->k = new_k;
    new_h->n = n;

    new_h->sommets = (Sommet**)calloc(new_nb_sommets, sizeof(Sommet*));
    if (new_h->sommets == NULL) {
        free(new_h);
        return NULL;
    }

    for (int i = 0; i < new_nb_sommets; i++) {
        new_h->sommets[i] = creer_sommet(i + 1);
    }

    new_h->g = creer_graphe(new_nb_sommets, new_M);
    if (new_h->g == NULL) {
        liberer_hypergraphe(new_h);
        return NULL;
    }

    Sommet** arete_courante = (Sommet**)malloc(new_k * sizeof(Sommet*));
    if (arete_courante == NULL) {
        liberer_hypergraphe(new_h);
        return NULL;
    }

    for (int i = 0; i < old_M; i++) {
        // Retrouver les sommets de l'ancienne arête i
        int num_old_vertices = 0;
        int* old_vertices_indices = (int*)malloc(old_k * sizeof(int));
        
        for (int v = 0; v < h->g->nb_sommets; v++) {
            if (h->g->matrice_incidence[i][v] == 1) {
                old_vertices_indices[num_old_vertices++] = v;
            }
        }

        // Duplication de l'arête n fois, avec ajout des n nouveaux sommets
        for (int c = 0; c < n; c++) {
            int new_edge_idx = i * n + c;
            
            // On réaffecte les anciens sommets depuis le nouveau tableau de sommets
            for (int j = 0; j < old_k; j++) {
                arete_courante[j] = new_h->sommets[old_vertices_indices[j]];
            }
            // On ajoute le nouveau sommet de la nouvelle partition
            arete_courante[old_k] = new_h->sommets[old_k * n + c];
            
            if (assigner_aretes_au_label(new_h->g, new_edge_idx, new_k, arete_courante) != 0) {
                free(old_vertices_indices);
                free(arete_courante);
                liberer_hypergraphe(new_h);
                return NULL;
            }
        }
        free(old_vertices_indices);
    }
    
    free(arete_courante);
    return new_h;
}

/*
 * Fonction pour inverser la dernière arête avec l'arête qui prend
 * le sommet du milieu pour la dernière partition (utilisé quand n est impair).
 * S'applique directement sur la matrice d'incidence.
 */
static void inverser_derniere_et_milieu_matrice(aGraphe* g, int n) {
    if (n % 2 == 0) return; // Uniquement pour n impair

    int index_derniere = g->n - 1; // g->n est le nombre d'arêtes M
    int index_milieu = g->n - 1 - (n / 2);

    int* temp = g->matrice_incidence[index_derniere];
    g->matrice_incidence[index_derniere] = g->matrice_incidence[index_milieu];
    g->matrice_incidence[index_milieu] = temp;
}

/*
 * Construit l'hypergraphe itérativement en partant de k=2 et en ajoutant des parties.
 * Vérifie et corrige la propriété anti-magique à chaque étape.
 */
HyperGrapheKParti* construire_hypergraphe_kparti(int k, int n) {
    if (k < 2 || n < 1) return NULL;

    HyperGrapheKParti* h = construire_hypergraphe_biparti(n);
    if (h == NULL) return NULL;

    // Test antimagique pour la base k=2
    calculer_sommes_sommets(h->g, h->sommets);
    if (!est_antimagique(h->sommets, h->k * h->n)) {
        inverser_derniere_et_milieu_matrice(h->g, n);
    }

    // Ajout en boucle des partitions jusqu'à atteindre k
    while (h->k < k) {
        HyperGrapheKParti* next_h = ajouter_partition_hypergraphe(h);
        if (next_h == NULL) {
            liberer_hypergraphe(h);
            return NULL;
        }

        // Test antimagique après l'ajout de la partition
        calculer_sommes_sommets(next_h->g, next_h->sommets);
        if (!est_antimagique(next_h->sommets, next_h->k * next_h->n)) {
            inverser_derniere_et_milieu_matrice(next_h->g, n);
        }

        liberer_hypergraphe(h); // On libère l'ancien hypergraphe
        h = next_h;             // On passe au suivant
    }

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
