#ifndef HYPER_KPARTI_H
#define HYPER_KPARTI_H

#include "agrah.h"

/*
 * Structure dédiée aux hypergraphes complets k-partis.
 * Permet de conserver le graphe sous-jacent et la liste des sommets sans modifier les structures de base.
 */
typedef struct {
    aGraphe* g;       // Le graphe (structure matricielle d'incidence gérant les arêtes)
    Sommet** sommets; // Le tableau global contenant tous les sommets alloués
    int k;            // Nombre de parties
    int n;            // Nombre de sommets par partie
} HyperGrapheKParti;

/*
 * Construit le cas de base : un hypergraphe biparti (k=2) avec n sommets par partie.
 * Les arêtes sont générées dans l'ordre lexicographique.
 */
HyperGrapheKParti* construire_hypergraphe_biparti(int n);

/*
 * Prend un hypergraphe k-parti existant et lui ajoute une partition (k devient k+1).
 * Utilise l'algorithme de duplication : chaque ancienne arête est dupliquée n fois
 * et on lui associe les n nouveaux sommets.
 * Retourne le nouvel hypergraphe (l'ancien doit être libéré par l'appelant).
 */
HyperGrapheKParti* ajouter_partition_hypergraphe(HyperGrapheKParti* h);

/*
 * Construit un hypergraphe complet k-parti où chaque partie contient n sommets.
 * Une hyper-arête contient exactement 1 sommet de chaque partie (donc k sommets par hyper-arête).
 * Le nombre total d'hyper-arêtes générées sera M = n^k.
 * 
 * - k : nombre de parties
 * - n : nombre de sommets par partie
 * 
 * Retourne la structure HyperGrapheKParti créée (contenant graphe et sommets) ou NULL en cas d'erreur.
 */
HyperGrapheKParti* construire_hypergraphe_kparti(int k, int n);



/*
 * Libère proprement la mémoire d'un HyperGrapheKParti et de tout son contenu.
 */
void liberer_hypergraphe(HyperGrapheKParti* h);

#endif // HYPER_KPARTI_H
