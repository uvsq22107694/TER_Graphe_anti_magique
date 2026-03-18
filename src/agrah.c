#include <stdio.h>
#include <stdlib.h>
#include "../include/agrah.h"

/*
 * Fonction pour allouer et initialiser un nouveau sommet.
 * Retourne un pointeur vers le sommet créé, ou NULL en cas d'échec d'allocation.
 */
Sommet* creer_sommet(int id) {
    Sommet* nouveau_sommet = (Sommet*)malloc(sizeof(Sommet)); // Allocation de la structure Sommet
    
    // Vérification stricte du retour de malloc pour la robustesse
    if (nouveau_sommet == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire pour le sommet %d.\n", id);
        return NULL;
    }
    
    // Initialisation
    nouveau_sommet->id = id;
    nouveau_sommet->valeur = 0;
    
    return nouveau_sommet;
}

/*
 * Fonction pour créer un graphe avec un tableau de n arêtes/hyper-arêtes.
 * Retourne un pointeur vers aGraphe, ou NULL en cas d'échec d'allocation.
 */
aGraphe* creer_graphe(int n) {
    // 1. Allocation de la structure principale du graphe
    aGraphe* g = (aGraphe*)malloc(sizeof(aGraphe));
    if (g == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire pour le graphe (aGraphe).\n");
        return NULL;
    }
    
    g->n = n;
    
    // 2. Allocation du tableau dynamique des labels (LienArrete)
    g->labels = (LienArrete*)malloc(n * sizeof(LienArrete));
    if (g->labels == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire pour les labels (LienArrete).\n");
        free(g); // En cas d'erreur de la deuxième allocation, on doit libérer la première !
        return NULL;
    }
    
    // 3. Initialisation rigoureusement à "vide" (0 et NULL) pour chaque case
    for (int i = 0; i < n; i++) {
        g->labels[i].nb_sommets = 0;
        g->labels[i].sommets = NULL;
    }
    
    return g;
}

/*
 * Fonction pour construire une arête ou hyper-arête en l'associant à un label (index).
 * - g : le graphe concerné
 * - label_index : l'index dans le tableau (de 0 à g->n - 1)
 * - nb_sommets : le nombre de sommets que cette arête va relier
 * - sommets_lies : un tableau de pointeurs contenant les adresses des sommets à lier
 * 
 * Retourne 0 en cas de succès, -1 en cas d'erreur.
 */
int assigner_aretes_au_label(aGraphe* g, int label_index, int nb_sommets, Sommet** sommets_lies) {
    // 1. Vérifications de sécurité
    if (g == NULL || sommets_lies == NULL) {
        fprintf(stderr, "Erreur : graphe ou tableau de sommets NULL.\n");
        return -1;
    }
    
    // Vérifier que le label_index (l'index de l'arête) est bien dans les limites du tableau
    if (label_index < 0 || label_index >= g->n) {
        fprintf(stderr, "Erreur : index de label %d invalide. Doit etre entre 0 et %d.\n", label_index, g->n - 1);
        return -1;
    }
    
    if (nb_sommets <= 0) {
        fprintf(stderr, "Erreur : nombre de sommets invalide.\n");
        return -1;
    }

    // 2. Allouer la mémoire pour le tableau de pointeurs vers les sommets liés à cette arête
    g->labels[label_index].sommets = (Sommet**)malloc(nb_sommets * sizeof(Sommet*));
    if (g->labels[label_index].sommets == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire pour les sommets de l'arete d'index %d.\n", label_index);
        return -1; // Échec d'allocation
    }
    
    // 3. Copier les pointeurs du tableau passé en argument
    for (int i = 0; i < nb_sommets; i++) {
        g->labels[label_index].sommets[i] = sommets_lies[i];
    }
    
    // 4. Mettre à jour le nombre de sommets pour cette arête spécifique
    g->labels[label_index].nb_sommets = nb_sommets;
    
    return 0; // Succès
}

/*
 * Fonction pour parcourir toutes les arêtes et calculer la valeur de chaque sommet.
 * La valeur ajoutée correspond au label de l'arête (index + 1).
 */
void calculer_sommes_sommets(aGraphe* g) {
    if (g == NULL) return;

    // 1. Remise à zéro des valeurs de tous les sommets liés présents dans le graphe
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->labels[i].nb_sommets; j++) {
            if (g->labels[i].sommets[j] != NULL) {
                g->labels[i].sommets[j]->valeur = 0;
            }
        }
    }

    // 2. Calcul des sommes en ajoutant (label_index + 1)
    for (int i = 0; i < g->n; i++) {
        int valeur_etiquette = i + 1; // La valeur de l'étiquette commence à 1
        
        for (int j = 0; j < g->labels[i].nb_sommets; j++) {
            if (g->labels[i].sommets[j] != NULL) {
                g->labels[i].sommets[j]->valeur += valeur_etiquette;
            }
        }
    }
}

// Fonction utilitaire de comparaison obligatoire pour qsort (tri croissant)
int comparer_entiers(const void* a, const void* b) {
    int int_a = *((int*)a);
    int int_b = *((int*)b);
    if (int_a == int_b) return 0;
    else if (int_a < int_b) return -1;
    else return 1;
}

/*
 * Fonction qui vérifie que toutes les sommes (valeurs de sommet) sont strictement uniques.
 * Retourne 1 si la propriété est respectée (graphe antimagique), 0 sinon.
 */
int est_antimagique(Sommet** tableau_tous_les_sommets, int nb_total_sommets) {
    if (tableau_tous_les_sommets == NULL || nb_total_sommets <= 0) {
        return 0; // Cas d'erreur : retour par défaut faux
    }

    // 1. Allouer un tableau temporaire pour stocker les valeurs
    int* valeurs_sommes = (int*)malloc(nb_total_sommets * sizeof(int));
    if (valeurs_sommes == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire dans est_antimagique.\n");
        return 0;
    }

    // 2. Extraire toutes les sommes des sommets
    for (int i = 0; i < nb_total_sommets; i++) {
        if (tableau_tous_les_sommets[i] != NULL) {
            valeurs_sommes[i] = tableau_tous_les_sommets[i]->valeur;
        } else {
            valeurs_sommes[i] = 0; // Sécurité si un sommet est NULL
        }
    }

    // 3. Trier le tableau de sommes de façon croissante avec qsort standard
    qsort(valeurs_sommes, nb_total_sommets, sizeof(int), comparer_entiers);

    // 4. Parcourir le tableau trié pour vérifier l'absence totale de doublons
    int aucun_doublon = 1; // 1 = Vrai
    for (int i = 1; i < nb_total_sommets; i++) {
        // En comparant la case i avec la case précédente i-1
        if (valeurs_sommes[i] == valeurs_sommes[i - 1]) {
            aucun_doublon = 0; // On a trouvé une égalité (doublon)
            break;
        }
    }

    // 5. Libérer le tableau mémoire temporaire
    free(valeurs_sommes);

    return aucun_doublon;
}

/*
 * Fonction pour libérer proprement et totalement la mémoire occupée par le graphe
 * ET par les sommets alloués en dehors.
 */
void liberer_graphe(aGraphe* g, Sommet** tableau_tous_les_sommets, int nb_total_sommets) {
    // Il est toujours bon de s'assurer que le pointeur n'est pas NULL avant de le manipuler
    if (g != NULL) {
        if (g->labels != NULL) {
            // 1. Libérer le tableau interne des sommets (adresses) dans chaque LienArrete !!
            // On ne libère pas les vrais sommets ici, seulement les tableaux de pointeurs.
            for (int i = 0; i < g->n; i++) {
                if (g->labels[i].sommets != NULL) {
                    free(g->labels[i].sommets); 
                }
            }
            // 2. Libérer le grand tableau "labels" (qui contient les LienArrete)
            free(g->labels);
        }
        // 3. Enfin, on libère la structure elle-même aGraphe
        free(g);
    }
    
    // 4. On s'occupe de libérer les vrais sommets (les données concrètes)
    // stockées généralement dans ce tableau global
    if (tableau_tous_les_sommets != NULL) {
        for (int i = 0; i < nb_total_sommets; i++) {
            if (tableau_tous_les_sommets[i] != NULL) {
                free(tableau_tous_les_sommets[i]); // Libérer chaque sommet
            }
        }
        // 5. Et on libère le tableau conteneur qui contenait tous les pointeurs
        free(tableau_tous_les_sommets);
    }
}
