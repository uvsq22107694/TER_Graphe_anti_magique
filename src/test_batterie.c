#include <stdio.h>
#include <stdlib.h>
#include "../include/agrah.h"
#include "../include/biparti.h"
#include "../include/kparti.h"
#include "../include/hyper_kparti.h"
#include "../include/test_batterie.h"

/*
 * Fonction utilitaire de test pour les graphes bipartis.
 */
void tester_biparti(int m, int n) {
    printf("[TEST BIPARTI] K_{%d,%d}... ", m, n);
    
    Sommet** tableau_sommets = NULL;
    // Construction du graphe biparti selon le Lemme 5.1
    aGraphe* g = construire_graphe_biparti_antimagique(m, n, &tableau_sommets);
    
    if (g == NULL || tableau_sommets == NULL) {
        printf("[ECHEC] Erreur lors de la creation du graphe.\n");
        return;
    }
    
    // Calcul des poids provisoires pour chaque sommet
    calculer_sommes_sommets(g, tableau_sommets);
    
    // Vérification mathématique via l'algorithme de tri et détection de collision
    int est_am = est_antimagique(tableau_sommets, m + n);
    
    if (est_am == 1) {
        printf("[SUCCES] Le graphe est antimagique !\n");
    } else {
        printf("[ECHEC] Collision detectee !\n");
    }
    
    // Libération totale de la mémoire sans fuite
    liberer_graphe(g, tableau_sommets, m + n);
}

/*
 * Lance une batterie de tests prédéfinis.
 */
void lancer_batterie_biparti() {
    printf("\n==== BATTERIE DE TESTS BIPARTIS (Lemme 5.1) ====\n");
    tester_biparti(2, 4);
    tester_biparti(3, 5);
    tester_biparti(4, 4);
    tester_biparti(10, 15);
    printf("================================================\n");
}

/*
 * Fonction utilitaire de test pour les hypergraphes k-partis.
 */
void tester_hyper_kparti(int k, int n) {
    printf("[TEST HYPER K-PARTI] k=%d, n=%d... ", k, n);
    
    HyperGrapheKParti* h = construire_hypergraphe_kparti(k, n);
    
    if (h == NULL || h->g == NULL || h->sommets == NULL) {
        printf("[ECHEC] Erreur lors de la creation de l'hypergraphe.\n");
        return;
    }
    
    int total_sommets = k * n;
    int M = 1;
    for (int i = 0; i < k; i++) M *= n;
    
    // Verifications basiques
    if (h->g->nb_sommets == total_sommets && h->g->n == M) {
        printf("[SUCCES] Structure creee avec %d sommets et %d hyper-aretes.\n", total_sommets, M);
        
        printf("\n--- Liste des aretes ---\n");
        for (int i = 0; i < h->g->n; i++) {
            printf("Arete %d : { ", i + 1);
            int premier = 1;
            for (int v = 0; v < h->g->nb_sommets; v++) {
                if (h->g->matrice_incidence[i][v] == 1) {
                    if (!premier) printf(", ");
                    char part_name = 'A' + (v / n);
                    int part_idx = (v % n) + 1;
                    printf("%c%d", part_name, part_idx);
                    premier = 0;
                }
            }
            printf(" }\n");
        }

        // Calcul des sommes pour chaque sommet
        calculer_sommes_sommets(h->g, h->sommets);
        
        printf("\n--- Valeurs (sommes) de chaque sommet ---\n");
        for (int p = 0; p < k; p++) {
            printf("Partition %d : ", p + 1);
            for (int i = 0; i < n; i++) {
                Sommet* s = h->sommets[p * n + i];
                printf("V%d=%d ", s->id, s->valeur);
            }
            printf("\n");
        }        
        // Vérification de la propriété anti-magique
        if (est_antimagique(h->sommets, total_sommets)) {
            printf("--> [RESULTAT] L'hypergraphe EST antimagique !\n");
        } else {
            printf("--> [RESULTAT] L'hypergraphe N'EST PAS antimagique (des doublons existent).\n");
        }
    } else {
        printf("[ECHEC] Dimensions incorrectes. Sommets: %d (attendu %d), Aretes: %d (attendu %d).\n", 
               h->g->nb_sommets, total_sommets, h->g->n, M);
    }
    
    printf("\n\n");
    liberer_hypergraphe(h);
}



/*
 * Lance une batterie de tests hypergraphes k-partis.
 */
void lancer_batterie_hyper_kparti() {
    printf("\n==== BATTERIE DE TESTS HYPERGRAPHES K-PARTIS ====\n");
    tester_hyper_kparti(4, 13);
    printf("=================================================\n");
}

/*
 * Fonction utilitaire de test pour les graphes k-partis.
 */
void tester_kparti(int* tailles, int k) {
    int total_sommets = 0;
    for (int i = 0; i < k; i++) {
        total_sommets += tailles[i];
    }
    
    printf("[TEST K-PARTI] k=%d avec un total de %d sommets.\n", k, total_sommets);
    
    Sommet** tableau_sommets = NULL;
    aGraphe* g = construire_graphe_kparti(tailles, k, &tableau_sommets);
    
    if (g == NULL || tableau_sommets == NULL) {
        printf("[ECHEC] Erreur lors de la creation du graphe.\n");
        return;
    }
    
    calculer_sommes_sommets(g, tableau_sommets);
    
    int est_am = est_antimagique(tableau_sommets, total_sommets);
    
    if (est_am == 1) {
        printf("[SUCCES] Le graphe est antimagique !\n");
    } else {
        printf("[ECHEC] Collision detectee ou echec de verification !\n");
    }
    
    liberer_graphe(g, tableau_sommets, total_sommets);
}

/*
 * Lance une batterie de tests k-partis.
 */
void lancer_batterie_kparti() {
    printf("\n==== BATTERIE DE TESTS K-PARTIS (Lemme 5.2) ====\n");
    
    // Test 1 : Un graphe 3-parti avec les tailles {2, 3, 4}
    int tailles1[] = {2, 3, 4};
    tester_kparti(tailles1, 3);
    
    // Test 2 : Un graphe 4-parti équilibré avec les tailles {2, 2, 2, 2}
    int tailles2[] = {2, 2, 2, 2};
    tester_kparti(tailles2, 4);
    
    // Test 3 : Un graphe 4-parti asymétrique avec les tailles {3, 5, 7, 9}
    int tailles3[] = {3, 5, 7, 9};
    tester_kparti(tailles3, 4);
    
    printf("================================================\n");
}
