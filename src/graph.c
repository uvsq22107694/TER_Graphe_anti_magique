#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

// Créer un nouveau graphe vide
Graphe* creer_graphe() {
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe));
    if (!graphe) {
        perror("Échec de l'allocation mémoire pour le graphe");
        exit(EXIT_FAILURE);
    }
    graphe->listeSommets = NULL;
    graphe->listeAretes = NULL;
    return graphe;
}

// Créer et ajouter un sommet au graphe
Sommet* ajouter_sommet(Graphe* graphe, int id) {
    Sommet* nouveauSommet = (Sommet*)malloc(sizeof(Sommet));
    if (!nouveauSommet) {
        perror("Échec de l'allocation mémoire pour le sommet");
        exit(EXIT_FAILURE);
    }
    nouveauSommet->id = id;
    nouveauSommet->aretes = NULL;
    nouveauSommet->suivant = graphe->listeSommets; // Insertion en tête
    graphe->listeSommets = nouveauSommet;
    return nouveauSommet;
}

// Créer et ajouter une arête au graphe
Arete* ajouter_arete(Graphe* graphe, int poids) {
    Arete* nouvelleArete = (Arete*)malloc(sizeof(Arete));
    if (!nouvelleArete) {
        perror("Échec de l'allocation mémoire pour l'arête");
        exit(EXIT_FAILURE);
    }
    nouvelleArete->poids = poids;
    nouvelleArete->sommets = NULL;
    nouvelleArete->suivant = graphe->listeAretes; // Insertion en tête
    graphe->listeAretes = nouvelleArete;
    return nouvelleArete;
}

// Ajouter un sommet à une arête existante
void ajouter_sommet_a_arete(Arete* arete, Sommet* sommet) {
    if (!arete || !sommet) return;

    // Ajouter le sommet à la liste des sommets de l'arête
    SommetNode* nodeSommet = (SommetNode*)malloc(sizeof(SommetNode));
    nodeSommet->sommet = sommet;
    nodeSommet->suivant = arete->sommets;
    arete->sommets = nodeSommet;

    // Ajouter l'arête à la liste des arêtes du sommet (pour référence inverse si besoin)
    AreteNode* nodeArete = (AreteNode*)malloc(sizeof(AreteNode));
    nodeArete->arete = arete;
    nodeArete->suivant = sommet->aretes;
    sommet->aretes = nodeArete;
}

// Trouver un sommet par son ID
Sommet* trouver_sommet(Graphe* graphe, int id) {
    Sommet* courant = graphe->listeSommets;
    while (courant) {
        if (courant->id == id) return courant;
        courant = courant->suivant;
    }
    return NULL;
}

// Afficher le contenu du graphe
void afficher_graphe(Graphe* graphe) {
    printf("=== Structure du Hypergraphe ===\n");
    
    printf("\nSOMMETS:\n");
    Sommet* s = graphe->listeSommets;
    while (s) {
        printf("Sommet ID: %d\n", s->id);
        s = s->suivant;
    }

    printf("\nARETES (Hyperaretes):\n");
    Arete* a = graphe->listeAretes;
    while (a) {
        printf("Arete Poids: %d connecte les sommets: { ", a->poids);
        SommetNode* sn = a->sommets;
        while (sn) {
            printf("%d ", sn->sommet->id);
            sn = sn->suivant;
        }
        printf("}\n");
        a = a->suivant;
    }
    printf("================================\n");
}

// Libérer la mémoire du graphe
void liberer_graphe(Graphe* graphe) {
    // Libérer les sommets et leurs listes d'arêtes associées
    Sommet* s = graphe->listeSommets;
    while (s) {
        Sommet* tempS = s;
        // Libérer la liste des arêtes connectées à ce sommet
        AreteNode* an = s->aretes;
        while (an) {
            AreteNode* tempAn = an;
            an = an->suivant;
            free(tempAn);
        }
        s = s->suivant;
        free(tempS);
    }

    // Libérer les arêtes et leurs listes de sommets associés
    Arete* a = graphe->listeAretes;
    while (a) {
        Arete* tempA = a;
        // Libérer la liste des sommets connectés par cette arête
        SommetNode* sn = a->sommets;
        while (sn) {
            SommetNode* tempSn = sn;
            sn = sn->suivant;
            free(tempSn);
        }
        a = a->suivant;
        free(tempA);
    }

    free(graphe);
}
