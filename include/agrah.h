#ifndef AGRAH_H
#define AGRAH_H

/*
 * Structure représentant un point/sommet du graphe.
 */
typedef struct Sommet {
    int id;          // Identifiant unique du sommet
    int valeur;      // Valeur représentant la somme des labels des arêtes connectées, calculée plus tard
} Sommet;

/*
 * Structure qui représente le lien créé par une arête (ou hyper-arête).
 * Elle contient un tableau dynamique d'adresses (pointeurs) vers les sommets qu'elle relie.
 * Cela permet de représenter des graphes classiques (2 sommets) ou des hypergraphes (plus de 2 sommets).
 */
typedef struct LienArrete {
    int nb_sommets;     // Nombre de sommets reliés par cette arête/hyper-arête
    Sommet** sommets;   // Tableau dynamique de pointeurs vers les sommets
} LienArrete;

/*
 * Structure principale aGraphe.
 * Contient un tableau "labels" de taille n, qui associe à chaque label (de 1 à n) l'arête correspondante.
 */
typedef struct aGraphe {
    int n;              // Le nombre d'arêtes (et donc la valeur maximale du label)
    LienArrete* labels; // Tableau dynamique. La case d'index i (de 0 à n-1) représentera l'arête de label i+1.
                        // Chaque case fait le lien vers les sommets reliés.
} aGraphe;

/*
 * Fonction pour allouer et initialiser un nouveau sommet.
 * Retourne un pointeur vers le sommet créé, ou NULL en cas d'échec d'allocation.
 */
Sommet* creer_sommet(int id);

/*
 * Fonction pour créer un graphe avec un tableau de n arêtes/hyper-arêtes.
 * Retourne un pointeur vers aGraphe, ou NULL en cas d'échec d'allocation.
 */
aGraphe* creer_graphe(int n);

/*
 * Assigne les sommets liés au label spécifique de l'arête (ou hyper-arête).
 * Retourne 0 en cas de succès, -1 en cas d'erreur.
 */
int assigner_aretes_au_label(aGraphe* g, int label_index, int nb_sommets, Sommet** sommets_lies);

/*
 * Parcourt toutes les arêtes, et pour chacune, ajoute l'index + 1 à la valeur
 * de chaque sommet qui lui est lié. (Remet à 0 les valeurs avant de calculer).
 */
void calculer_sommes_sommets(aGraphe* g);

/*
 * Vérifie si les sommes calculées dans les sommets sont toutes uniques.
 * Retourne 1 si elles sont uniques (propriété anti-magique respectée), sinon 0.
 */
int est_antimagique(Sommet** tableau_tous_les_sommets, int nb_total_sommets);

/*
 * Fonction pour libérer proprement et totalement la mémoire occupée par le graphe
 * ET par les sommets alloués en dehors.
 */
void liberer_graphe(aGraphe* g, Sommet** tableau_tous_les_sommets, int nb_total_sommets);

#endif // AGRAH_H