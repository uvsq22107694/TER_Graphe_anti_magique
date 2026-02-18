#ifndef ANTIMAGIC_H
#define ANTIMAGIC_H

#include "graph.h"
#include <stdbool.h>

/**
 * Cherche un étiquetage anti-magique sur les arêtes.
 * Assigne des poids de 1 à M aux arêtes tel que les sommes des poids incidents
 * à chaque sommet soient distinctes.
 * 
 * Modifie directement la matrice d'adjacence du graphe avec les poids trouvés.
 * 
 * @param g Le graphe à étiqueter
 * @return true si un étiquetage a été trouvé, false sinon
 */
bool trouver_etiquetage_antimagique(Graphe* g);

/**
 * Applique un étiquetage glouton décroissant.
 * Algorithme :
 * 1. k = M (nombre d'arêtes)
 * 2. Tant que k > 0 :
 *    - Choisir le sommet v maximisant : (somme des poids des arêtes adjacentes déjà pondérées) + (k * nombre d'arêtes adjacentes non pondérées)
 *    - Assigner aux arêtes incidentes non pondérées de v les poids k, k-1, ...
 *    - Mettre à jour k
 */
void etiquetage_glouton_decroissant(Graphe* g);

/**
 * Applique un étiquetage basé sur le degré croissant.
 * Algorithme :
 * 1. k = 1 (poids le plus faible)
 * 2. Trier les sommets par degré croissant.
 * 3. Pour chaque sommet v de la liste triée :
 *    - Assigner aux arêtes incidentes non pondérées les poids k, k+1, ...
 *    - Mettre à jour k
 */
void etiquetage_degre_croissant(Graphe* g);

#endif
