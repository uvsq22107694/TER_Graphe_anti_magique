#include <stdio.h>
#include <stdlib.h>
#include "../include/agrah.h"
#include "../include/biparti.h"
#include "../include/kparti.h"
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
