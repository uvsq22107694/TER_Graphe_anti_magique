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

#endif
