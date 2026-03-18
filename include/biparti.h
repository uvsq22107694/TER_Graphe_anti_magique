#ifndef BIPARTI_H
#define BIPARTI_H

#include "agrah.h"

/*
 * Génère une matrice 2D de taille m x n remplie avec les valeurs de 1 à m*n.
 * Le remplissage suit les règles du Lemme 5.1 de N. Alon pour les graphes bipartis.
 * - Les lignes paires (index pair) et la dernière ligne sont remplies de gauche à droite.
 * - Les lignes impaires (sauf la dernière) sont remplies de droite à gauche.
 */
int** generer_matrice_base(int m, int n);

/*
 * Libère la mémoire allouée pour la matrice 2D.
 */
void liberer_matrice(int** matrice, int m);

/*
 * Résout la collision unique (s'il y en a) entre la somme d'une ligne
 * et la somme d'une colonne (selon le Lemme 5.1 de N. Alon).
 */
void resoudre_collision_matrice(int** matrice, int m, int n);

/*
 * Fonction finale qui fait le lien entre la matrice mathématique
 * et la structure de données du graphe.
 */
aGraphe* construire_graphe_biparti_antimagique(int m, int n, Sommet*** out_tableau_sommets);

/*
 * Affiche la matrice dans la console.
 */
void afficher_matrice(int** matrice, int m, int n);

#endif // BIPARTI_H
