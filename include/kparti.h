#ifndef KPARTI_H
#define KPARTI_H

#include "agrah.h"

/*
 * Lemme 5.2 de N. Alon - Graphes k-partis complets.
 * 
 * Génère les arêtes internes du sous-ensemble B (composé des partitions 1 à k-1).
 * La partition 0 est l'ensemble A et est exclue ici.
 * Assigne les labels d'arêtes à partir du label 'start_label'.
 * 
 * - tailles_partitions: Tableau contenant la taille de chaque partition.
 * - k: Le nombre total de partitions (A inclus, donc k >= 2).
 * - g: Le graphe dans lequel ajouter les arêtes.
 * - tous_sommets: Tableau global des sommets du graphe complet.
 * - start_label: La valeur mathématique du premier label (1-based) à utiliser.
 * 
 * Retourne la valeur de q, le nombre d'arêtes générées et assignées.
 */
int generer_aretes_internes_B(int* tailles_partitions, int k, aGraphe* g, Sommet** tous_sommets, int start_label);

#endif // KPARTI_H
