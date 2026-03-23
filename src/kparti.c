#include <stdio.h>
#include <stdlib.h>
#include "../include/kparti.h"

int generer_aretes_internes_B(int* tailles_partitions, int k, aGraphe* g, Sommet** tous_sommets, int start_label) {
    if (tailles_partitions == NULL || g == NULL || tous_sommets == NULL || k < 2) {
        return 0; // Paramètres invalides
    }

    int q = 0; // Compteur du nombre d'arêtes créées

    // Parcourir toutes les paires possibles de partitions x et y dans B
    // B est l'ensemble des partitions de l'index 1 à k-1
    for (int x = 1; x < k - 1; x++) {
        for (int y = x + 1; y < k; y++) {
            
            // Calcul global de l'offset pour la partition x 
            // (on additionne la taille des partitions précédentes, y compris A=0)
            int offset_x = 0;
            for (int p = 0; p < x; p++) {
                offset_x += tailles_partitions[p];
            }
            
            // Calcul global de l'offset pour la partition y
            int offset_y = 0;
            for (int p = 0; p < y; p++) {
                offset_y += tailles_partitions[p];
            }
            
            // Créer une arête entre chaque sommet de x et chaque sommet de y
            for (int i = 0; i < tailles_partitions[x]; i++) {
                Sommet* sommet_x = tous_sommets[offset_x + i];
                
                for (int j = 0; j < tailles_partitions[y]; j++) {
                    Sommet* sommet_y = tous_sommets[offset_y + j];
                    
                    Sommet* arete_sommets[2] = { sommet_x, sommet_y };
                    
                    // L'index d'arête dans la matrice commence à start_label - 1
                    int label_index = (start_label - 1) + q;
                    
                    if (assigner_aretes_au_label(g, label_index, 2, arete_sommets) != 0) {
                        fprintf(stderr, "Erreur lors de l'assignation de l'arete dans generer_aretes_internes_B.\n");
                        return -1; // En cas d'erreur
                    }
                    
                    q++; // Incrémenter le nombre d'arêtes ajoutées
                }
            }
        }
    }
    
    return q; // Retourne le nombre total d'arêtes créées
}

/*
 * Fonction de comparaison compatible avec qsort.
 * Compare la valeur accumulée de deux sommets.
 */
int comparer_sommets(const void* a, const void* b) {
    // a et b sont des pointeurs vers les éléments du tableau, 
    // qui sont eux-mêmes des pointeurs de Sommet (Sommet**)
    Sommet* sommetA = *(Sommet**)a;
    Sommet* sommetB = *(Sommet**)b;
    
    // Si l'un des sommets est NULL, on le place à la fin (par sécurité)
    if (sommetA == NULL && sommetB == NULL) return 0;
    if (sommetA == NULL) return 1;
    if (sommetB == NULL) return -1;
    
    // Comparaison par ordre croissant de la valeur
    if (sommetA->valeur < sommetB->valeur) return -1;
    if (sommetA->valeur > sommetB->valeur) return 1;
    return 0; // Valeurs égales
}

/*
 * Trie le tableau sommets_B par ordre de poids croissant en utilisant qsort.
 * Cela permettra d'avoir la suite u_1, u_2, ..., u_m du Lemme 5.2.
 */
void trier_sommets_B(Sommet** sommets_B, int m) {
    if (sommets_B == NULL || m <= 1) {
        return; // Rien à trier
    }
    
    qsort(sommets_B, m, sizeof(Sommet*), comparer_sommets);
}

/*
 * Implémentation du Lemme 5.2 de N. Alon.
 */
aGraphe* construire_graphe_kparti(int* tailles_partitions, int k, Sommet*** out_tableau_sommets) {
    if (tailles_partitions == NULL || k < 2 || out_tableau_sommets == NULL) {
        return NULL;
    }

    int n_1 = tailles_partitions[0]; // Taille de la partition A
    int m = 0;                       // Taille de l'ensemble B
    for (int p = 1; p < k; p++) {
        m += tailles_partitions[p];
    }

    // 1. Calcul du nombre total d'arêtes (produit des tailles deux à deux)
    int nb_aretes = 0;
    for (int x = 0; x < k - 1; x++) {
        for (int y = x + 1; y < k; y++) {
            nb_aretes += tailles_partitions[x] * tailles_partitions[y];
        }
    }

    // 2. Allocation globale des sommets
    int nb_total_sommets = n_1 + m;
    Sommet** tous_sommets = (Sommet**)calloc(nb_total_sommets, sizeof(Sommet*));
    if (tous_sommets == NULL) return NULL;
    
    for (int i = 0; i < nb_total_sommets; i++) {
        tous_sommets[i] = creer_sommet(i + 1);
    }
    *out_tableau_sommets = tous_sommets;

    // 3. Création du graphe
    aGraphe* g = creer_graphe(nb_total_sommets, nb_aretes);
    if (g == NULL) {
        // En cas d'erreur de mémoire, on devrait libérer tous_sommets, mais on suppose un flux idéal
        return NULL;
    }

    // 4. Arêtes internes de B (labels allant de 1 à q)
    int q = generer_aretes_internes_B(tailles_partitions, k, g, tous_sommets, 1);
    if (q < 0) return NULL; // Gestion d'erreur

    // 5. Calculer les sommes partielles (poids provisoires)
    calculer_sommes_sommets(g, tous_sommets);

    // 6. Extraire et trier les sommets de B pour obtenir la séquence u_1, ..., u_m
    Sommet** sommets_B = (Sommet**)malloc(m * sizeof(Sommet*));
    if (sommets_B == NULL) return NULL;
    
    for (int j = 0; j < m; j++) {
        sommets_B[j] = tous_sommets[n_1 + j];
    }
    
    trier_sommets_B(sommets_B, m);

    // 7. Relier la partition A à l'ensemble B avec la construction magique
    for (int i = 1; i <= n_1; i++) {
        for (int j = 1; j <= m; j++) {
            int L;
            
            if (j % 2 != 0) { // j est impair
                L = (i - 1) * m + j + q;
            } else { // j est pair
                if (m % 2 == 0 && j == m) { // Exception pour le dernier élément si m est pair
                    L = i * m + q;
                } else { // Cas pair standard
                    L = (n_1 - i) * m + j + q;
                }
            }
            
            // Le sommet dans A est le sommet global à l'index i-1
            Sommet* v_i = tous_sommets[i - 1]; 
            // Le sommet dans B est le sommet trié à l'index j-1
            Sommet* u_j = sommets_B[j - 1];
            
            Sommet* arete_sommets[2] = { v_i, u_j };
            int label_index = L - 1;
            
            assigner_aretes_au_label(g, label_index, 2, arete_sommets);
        }
    }

    free(sommets_B); // Nettoyage du tableau temporaire

    return g;
}
