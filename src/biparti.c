#include <stdio.h>
#include <stdlib.h>
#include "../include/biparti.h"

/*
 * Génère une matrice 2D de taille m x n remplie avec les valeurs de 1 à m*n.
 * Le remplissage suit les règles du Lemme 5.1 de N. Alon :
 * Ligne i reçoit les valeurs (i * n) + 1 jusqu'à (i + 1) * n.
 * Si index i pair OU dernière ligne : gauche à droite.
 * Si index i impair et non dernière ligne : droite à gauche.
 */
int** generer_matrice_base(int m, int n) {
    // Allocation du tableau de pointeurs de lignes
    int** matrice = (int**)malloc(m * sizeof(int*));
    if (matrice == NULL) {
        fprintf(stderr, "Erreur d'allocation pour les lignes de la matrice.\n");
        return NULL;
    }

    for (int i = 0; i < m; i++) {
        // Allocation de chaque ligne
        matrice[i] = (int*)malloc(n * sizeof(int));
        if (matrice[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation pour les colonnes de la ligne %d.\n", i);
            // Libération des lignes déjà allouées avant de quitter
            for (int k = 0; k < i; k++) {
                free(matrice[k]);
            }
            free(matrice);
            return NULL;
        }

        // Valeur de départ pour cette ligne
        int valeur_courante = (i * n) + 1;

        // Remplissage selon l'index de ligne
        if (i % 2 == 0 || i == m - 1) {
            // Index pair OU dernière ligne : remplissage de gauche à droite
            for (int j = 0; j < n; j++) {
                matrice[i][j] = valeur_courante++;
            }
        } else {
            // Index impair ET pas la dernière ligne : remplissage de droite à gauche
            for (int j = n - 1; j >= 0; j--) {
                matrice[i][j] = valeur_courante++;
            }
        }
    }

    return matrice;
}

/*
 * Gère l'unique collision potentielle où la somme d'une ligne i
 * équivaut à la somme d'une colonne j.
 * - R : tableau des sommes de chaque ligne
 * - C : tableau des sommes de chaque colonne
 */
void resoudre_collision_matrice(int** matrice, int m, int n) {
    if (matrice == NULL || m < 2 || n < 4) return;

    // 1. Allocation des tableaux pour stocker les sommes
    int* R = (int*)calloc(m, sizeof(int)); // Sommes des lignes
    int* C = (int*)calloc(n, sizeof(int)); // Sommes des colonnes

    if (R == NULL || C == NULL) {
        fprintf(stderr, "Erreur d'allocation dans resoudre_collision_matrice.\n");
        if (R != NULL) free(R);
        if (C != NULL) free(C);
        return;
    }

    // 2. Calcul des sommes pour le tableau R (lignes) et C (colonnes)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            R[i] += matrice[i][j];
            C[j] += matrice[i][j];
        }
    }

    // 3. Recherche de la collision
    int collision_ligne = -1;
    // L'article précise que la collision ne peut s'appliquer que sur i < m - 1
    for (int i = 0; i < m - 1; i++) {
        for (int j = 0; j < n; j++) {
            if (R[i] == C[j]) {
                collision_ligne = i;
                break; // Collision trouvée !
            }
        }
        if (collision_ligne != -1) break;
    }

    // 4. Résolution de la collision selon les règles mathématiques de N. Alon
    if (collision_ligne != -1) {
        int i = collision_ligne;
        int temp; // Variable pour effectuer l'échange
        
        // Règle 1 : La ligne en collision est la première (index 0)
        if (i == 0) {
            // Échange matrice[1][0] et matrice[0][0]
            temp = matrice[1][0];
            matrice[1][0] = matrice[0][0];
            matrice[0][0] = temp;
        }
        // Règle 2 : La ligne en collision est une ligne impaire (i > 0)
        else if (i > 0 && i % 2 != 0) {
            // Échange matrice[i][0] et matrice[i-1][0]
            temp = matrice[i][0];
            matrice[i][0] = matrice[i - 1][0];
            matrice[i - 1][0] = temp;
        }
        // Règle 3 : La ligne en collision est une ligne paire (i > 0)
        else if (i > 0 && i % 2 == 0) {
            // Échange matrice[i][n-1] et matrice[i-1][n-1]
            temp = matrice[i][n - 1];
            matrice[i][n - 1] = matrice[i - 1][n - 1];
            matrice[i - 1][n - 1] = temp;
        }
    }

    // Libération de la mémoire des deux tableaux
    free(R);
    free(C);
}

/*
 * Libère la mémoire allouée pour la matrice 2D.
 */
void liberer_matrice(int** matrice, int m) {
    if (matrice != NULL) {
        for (int i = 0; i < m; i++) {
            if (matrice[i] != NULL) {
                free(matrice[i]);
            }
        }
        free(matrice);
    }
}

/*
 * Fonction finale qui fait le lien entre la matrice mathématique
 * et la structure de données du graphe.
 * - m : le nombre de sommets de la première partition (lignes)
 * - n : le nombre de sommets de la deuxième partition (colonnes)
 * - out_tableau_sommets : pointeur de retour pour récupérer le tableau des sommets global
 */
aGraphe* construire_graphe_biparti_antimagique(int m, int n, Sommet*** out_tableau_sommets) {
    // 1. Génération de la matrice base
    int** matrice = generer_matrice_base(m, n);
    if (matrice == NULL) return NULL;

    // 2. Résolution de la collision unique éventuelle
    resoudre_collision_matrice(matrice, m, n);
    
    // 3. Création du tableau contenant le total des sommets : (m + n) sommets
    int nb_total_sommets = m + n;
    Sommet** tous_les_sommets = (Sommet**)calloc(nb_total_sommets, sizeof(Sommet*));
    if (tous_les_sommets == NULL) {
        fprintf(stderr, "Erreur d'allocation pour le tableau global des sommets.\n");
        liberer_matrice(matrice, m);
        return NULL;
    }
    
    // Initialisation des sommets (Rappel: partition 1 (lignes) a ID 1..m, partition 2 (col) a ID m+1..m+n)
    for (int i = 0; i < nb_total_sommets; i++) {
        tous_les_sommets[i] = creer_sommet(i + 1);
    }
    
    // Passage du tableau de sommets à l'appelant via le paramètre de sortie
    *out_tableau_sommets = tous_les_sommets;

    // 4. Initialisation du graphe (le nombre d'arêtes = m * n)
    int nb_aretes = m * n;
    aGraphe* graphe = creer_graphe(nb_total_sommets, nb_aretes);
    if (graphe == NULL) {
        liberer_matrice(matrice, m);
        return NULL; // Erreur déjà affichée dans creer_graphe
    }

    // 5. Remplir le graphe selon la matrice finale générée
    // Dans la matrice, M[r][c] = label final de l'arête reliant le sommet (r+1) et le sommet (m+c+1)
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            // Lecture de la valeur de label depuis la matrice générée
            int valeur_label = matrice[r][c];
            
            // L'index dans le graphe (0 à m*n-1) est la valeur du label - 1
            int label_index = valeur_label - 1;
            
            // Les sommets à connecter
            // Ligne r correspond au sommet ID (r+1), qui se trouve à l'index r du tableau 
            Sommet* sommet_ligne = tous_les_sommets[r];
            
            // Colonne c correspond au sommet ID (m+c+1), qui se trouve à l'index (m+c) du tableau
            Sommet* sommet_colonne = tous_les_sommets[m + c];
            
            // Regroupement dans un tableau temporaire pour l'assigner à l'arête
            Sommet* arete_sommets[2] = {sommet_ligne, sommet_colonne};
            
            // Assigner ces deux sommets à l'arête d'index label_index 
            if (assigner_aretes_au_label(graphe, label_index, 2, arete_sommets) != 0) {
                fprintf(stderr, "Erreur critique : Echec d'assignation au label %d.\n", valeur_label);
                liberer_matrice(matrice, m);
                return NULL;
            }
        }
    }

    // 6. Nettoyage mathographique : la matrice n'est plus nécessaire car le graphe en C est complété !
    liberer_matrice(matrice, m);
    
    return graphe;
}

/*
 * Affiche la matrice dans la console avec alignement.
 */
void afficher_matrice(int** matrice, int m, int n) {
    if (matrice == NULL) return;
    
    printf("Matrice de base (%dx%d) :\n", m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", matrice[i][j]); // %4d pour un affichage aligné avec des espaces
        }
        printf("\n");
    }
}
