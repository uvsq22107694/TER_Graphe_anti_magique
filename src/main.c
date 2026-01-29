#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main() {
    printf("Projet de Modélisation de Graphe Anti-Magique (Hypergraphe)\n");

    // Création du graphe
    Graphe* g = creer_graphe();

    // Ajout de sommets (IDs arbitraires)
    Sommet* s1 = ajouter_sommet(g, 1);
    Sommet* s2 = ajouter_sommet(g, 2);
    Sommet* s3 = ajouter_sommet(g, 3);
    Sommet* s4 = ajouter_sommet(g, 4);
    Sommet* s5 = ajouter_sommet(g, 5);

    // Ajout d'arêtes (Hyperarêtes avec poids)
    
    // Arête de poids 10 connectant s1 et s2 (Arête simple)
    Arete* a1 = ajouter_arete(g, 10);
    ajouter_sommet_a_arete(a1, s1);
    ajouter_sommet_a_arete(a1, s2);

    // Arête de poids 20 connectant s2, s3 et s4 (Hyperarête)
    Arete* a2 = ajouter_arete(g, 20);
    ajouter_sommet_a_arete(a2, s2);
    ajouter_sommet_a_arete(a2, s3);
    ajouter_sommet_a_arete(a2, s4);

    // Arête de poids 10 (dupliqué) connectant s4 et s5
    Arete* a3 = ajouter_arete(g, 10);
    ajouter_sommet_a_arete(a3, s4);
    ajouter_sommet_a_arete(a3, s5);

    // Affichage
    afficher_graphe(g);

    // Libération de la mémoire
    liberer_graphe(g);

    return 0;
}
