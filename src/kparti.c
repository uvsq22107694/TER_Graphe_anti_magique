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
