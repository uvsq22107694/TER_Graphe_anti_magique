#ifndef GRAPH_H
#define GRAPH_H

// Structure pour représenter un maillon d'une liste chaînée de sommets
typedef struct SommetNode {
    struct Sommet* sommet;
    struct SommetNode* suivant;
} SommetNode;

// Structure pour représenter un sommet (Vertex)
typedef struct Sommet {
    int id;                     // Identifiant unique du sommet
    struct AreteNode* aretes;   // Liste des arêtes connectées à ce sommet
    struct Sommet* suivant;     // Suivant dans la liste globale des sommets
} Sommet;

// Structure pour représenter une arête (Edge) ou hyperarête
typedef struct Arete {
    int poids;                  // Poids de l'arête (non unique)
    SommetNode* sommets;        // Liste des sommets connectés par cette arête
    struct Arete* suivant;      // Suivant dans la liste globale des arêtes
} Arete;

// Structure pour représenter un maillon d'une liste chaînée d'arêtes
typedef struct AreteNode {
    Arete* arete;
    struct AreteNode* suivant;
} AreteNode;

// Structure pour représenter le graphe (hypergraphe)
typedef struct Graphe {
    Sommet* listeSommets;       // Tête de la liste des sommets
    Arete* listeAretes;         // Tête de la liste des arêtes
} Graphe;

// Prototypes de fonctions

// Créer un nouveau graphe vide
Graphe* creer_graphe();

// Créer et ajouter un sommet au graphe
Sommet* ajouter_sommet(Graphe* graphe, int id);

// Créer et ajouter une arête au graphe
Arete* ajouter_arete(Graphe* graphe, int poids);

// Ajouter un sommet à une arête existante
void ajouter_sommet_a_arete(Arete* arete, Sommet* sommet);

// Afficher le contenu du graphe
void afficher_graphe(Graphe* graphe);

// Libérer la mémoire du graphe
void liberer_graphe(Graphe* graphe);

// Trouver un sommet par son ID (utilitaire)
Sommet* trouver_sommet(Graphe* graphe, int id);

#endif
