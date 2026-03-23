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
 * Structure principale aGraphe.
 * Contient une matrice d'incidence sous forme de tableau de pointeurs de colonnes.
 * Cela permet de facilement permuter les arêtes en échangeant leurs pointeurs.
 */
typedef struct aGraphe {
    int nb_sommets;          // Nombre total de sommets dans le graphe
    int n;                   // Le nombre d'arêtes (et donc la valeur maximale du label)
    int** matrice_incidence; // matrice_incidence[i][j] = 1 si le sommet j (id j+1) appartient à l'arête i.
                             // La case d'index i (de 0 à n-1) représente l'arête de label i+1.
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
aGraphe* creer_graphe(int nb_sommets, int n);

/*
 * Assigne les sommets liés au label spécifique de l'arête (ou hyper-arête).
 * Retourne 0 en cas de succès, -1 en cas d'erreur.
 */
int assigner_aretes_au_label(aGraphe* g, int label_index, int nb_sommets, Sommet** sommets_lies);

/*
 * Parcourt toutes les arêtes, et pour chacune, ajoute l'index + 1 à la valeur
 * de chaque sommet qui lui est lié. (Remet à 0 les valeurs avant de calculer).
 */
void calculer_sommes_sommets(aGraphe* g, Sommet** tous_les_sommets);

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