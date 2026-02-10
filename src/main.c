#include <stdio.h>
#include <stdlib.h>
#include "hypergraph.h"
#include "graph.h"
#include "visu.h"

void test_hypergraphe_legacy() {
    printf("\n=== TEST HYPERGRAPHE LEGACY ===\n");
    // Création du graphe
    HyperGraphe* g = creer_hypergraphe();

    // Ajout de sommets (IDs arbitraires)
    HyperSommet* s1 = ajouter_hypersommet(g, 1);
    HyperSommet* s2 = ajouter_hypersommet(g, 2);
    HyperSommet* s3 = ajouter_hypersommet(g, 3);
    HyperSommet* s4 = ajouter_hypersommet(g, 4);
    HyperSommet* s5 = ajouter_hypersommet(g, 5);

    // Ajout d'arêtes (Hyperarêtes avec poids)
    
    // Arête de poids 10 connectant s1 et s2 (Arête simple)
    HyperArete* a1 = ajouter_hyperarete(g, 10);
    ajouter_sommet_a_hyperarete(a1, s1);
    ajouter_sommet_a_hyperarete(a1, s2);

    // Arête de poids 20 connectant s2, s3 et s4 (Hyperarête)
    HyperArete* a2 = ajouter_hyperarete(g, 20);
    ajouter_sommet_a_hyperarete(a2, s2);
    ajouter_sommet_a_hyperarete(a2, s3);
    ajouter_sommet_a_hyperarete(a2, s4);

    // Arête de poids 10 (dupliqué) connectant s4 et s5
    HyperArete* a3 = ajouter_hyperarete(g, 10);
    ajouter_sommet_a_hyperarete(a3, s4);
    ajouter_sommet_a_hyperarete(a3, s5);

    // Affichage
    afficher_hypergraphe(g);

    // Libération de la mémoire
    liberer_hypergraphe(g);
}

void test_graphe_antimagique() {
    printf("\n=== TEST NOUVEAU GRAPHE (ANTIMAGIQUE) ===\n");
    int n = 5;
    Graphe* g = creer_graphe(n);
    
    // Créons un cycle C5 : 0-1, 1-2, 2-3, 3-4, 4-0
    // Indices 0 à 4 correspondent aux sommets
    ajouter_arete(g, 0, 1);
    ajouter_arete(g, 1, 2);
    ajouter_arete(g, 2, 3);
    ajouter_arete(g, 3, 4);
    ajouter_arete(g, 4, 0);

    afficher_graphe(g);
    
    test_antimagique(g);

    visualiser_graphe(g);
    
    liberer_graphe(g);
}

int main() {
    test_hypergraphe_legacy();
    test_graphe_antimagique();
    return 0;
}
