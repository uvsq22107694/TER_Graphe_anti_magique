#ifndef ALGO_ANTIMAGIQUE_H
#define ALGO_ANTIMAGIQUE_H

#include "agrah.h"

/*
 * Algorithmes d'assignation de labels (poids) pour trouver une configuration antimagique.
 * Ces fonctions procèdent en échangeant les pointeurs de la matrice d'incidence.
 * 
 * g: pointeur vers l'hypergraphe
 * sommets: tableau des sommets
 * max_iter: nombre d'itérations maximum (généralement 1000 à 100000)
 * nb_swaps: compteur incrémenté pour suivre l'activité de l'algorithme
 * 
 * Retournent 1 si une solution antimagique a été trouvée, 0 sinon.
 */

int algo_aleatoire(aGraphe* g, Sommet** sommets, int max_iter, int* nb_swaps);

int algo_force_brute(aGraphe* g, Sommet** sommets, int* nb_swaps);

int algo_heuristique(aGraphe* g, Sommet** sommets, int max_iter, int* nb_swaps);

#endif // ALGO_ANTIMAGIQUE_H
