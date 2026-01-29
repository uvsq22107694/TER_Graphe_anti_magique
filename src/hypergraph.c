#include <stdio.h>
#include <stdlib.h>
#include "hypergraph.h"

// Créer un nouveau graphe vide
HyperGraphe* creer_hypergraphe() {
    HyperGraphe* graphe = (HyperGraphe*)malloc(sizeof(HyperGraphe));
    if (!graphe) {
        perror("Échec de l'allocation mémoire pour le graphe");
        exit(EXIT_FAILURE);
    }
    graphe->listeSommets = NULL;
    graphe->listeAretes = NULL;
    return graphe;
}

// Créer et ajouter un sommet au graphe
HyperSommet* ajouter_hypersommet(HyperGraphe* graphe, int id) {
    HyperSommet* nouveauSommet = (HyperSommet*)malloc(sizeof(HyperSommet));
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
HyperArete* ajouter_hyperarete(HyperGraphe* graphe, int poids) {
    HyperArete* nouvelleArete = (HyperArete*)malloc(sizeof(HyperArete));
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
void ajouter_sommet_a_hyperarete(HyperArete* arete, HyperSommet* sommet) {
    if (!arete || !sommet) return;

    // Ajouter le sommet à la liste des sommets de l'arête
    HyperSommetNode* nodeSommet = (HyperSommetNode*)malloc(sizeof(HyperSommetNode));
    nodeSommet->sommet = sommet;
    nodeSommet->suivant = arete->sommets;
    arete->sommets = nodeSommet;

    // Ajouter l'arête à la liste des arêtes du sommet (pour référence inverse si besoin)
    HyperAreteNode* nodeArete = (HyperAreteNode*)malloc(sizeof(HyperAreteNode));
    nodeArete->arete = arete;
    nodeArete->suivant = sommet->aretes;
    sommet->aretes = nodeArete;
}

// Trouver un sommet par son ID
HyperSommet* trouver_hypersommet(HyperGraphe* graphe, int id) {
    HyperSommet* courant = graphe->listeSommets;
    while (courant) {
        if (courant->id == id) return courant;
        courant = courant->suivant;
    }
    return NULL;
}

// Afficher le contenu du graphe
void afficher_hypergraphe(HyperGraphe* graphe) {
    printf("=== Structure du Hypergraphe ===\n");
    
    printf("\nSOMMETS:\n");
    HyperSommet* s = graphe->listeSommets;
    while (s) {
        printf("Sommet ID: %d\n", s->id);
        s = s->suivant;
    }

    printf("\nARETES (Hyperaretes):\n");
    HyperArete* a = graphe->listeAretes;
    while (a) {
        printf("Arete Poids: %d connecte les sommets: { ", a->poids);
        HyperSommetNode* sn = a->sommets;
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
void liberer_hypergraphe(HyperGraphe* graphe) {
    // Libérer les sommets et leurs listes d'arêtes associées
    HyperSommet* s = graphe->listeSommets;
    while (s) {
        HyperSommet* tempS = s;
        // Libérer la liste des arêtes connectées à ce sommet
        HyperAreteNode* an = s->aretes;
        while (an) {
            HyperAreteNode* tempAn = an;
            an = an->suivant;
            free(tempAn);
        }
        s = s->suivant;
        free(tempS);
    }

    // Libérer les arêtes et leurs listes de sommets associés
    HyperArete* a = graphe->listeAretes;
    while (a) {
        HyperArete* tempA = a;
        // Libérer la liste des sommets connectés par cette arête
        HyperSommetNode* sn = a->sommets;
        while (sn) {
            HyperSommetNode* tempSn = sn;
            sn = sn->suivant;
            free(tempSn);
        }
        a = a->suivant;
        free(tempA);
    }

    free(graphe);
}
