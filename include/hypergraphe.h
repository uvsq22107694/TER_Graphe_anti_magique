#ifndef HYPERGRAPHE_H
#define HYPERGRAPHE_H

#include "agrah.h"

/*
 * Génère un hypergraphe valide de manière aléatoire.
 * S'assure qu'aucun sommet ne possède exactement le même ensemble d'arêtes.
 * Retourne le graphe créé et alloue le tableau de sommets.
 */
aGraphe* generer_hypergraphe_valide(int nb_sommets, int nb_aretes, Sommet*** tableau_sommets);

#endif // HYPERGRAPHE_H
