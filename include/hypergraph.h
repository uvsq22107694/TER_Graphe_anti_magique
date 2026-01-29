#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H

// Structure pour représenter un maillon d'une liste chaînée de sommets
typedef struct HyperSommetNode {
    struct HyperSommet* sommet;
    struct HyperSommetNode* suivant;
} HyperSommetNode;

// Structure pour représenter un sommet (Vertex)
typedef struct HyperSommet {
    int id;                     // Identifiant unique du sommet
    struct HyperAreteNode* aretes;   // Liste des arêtes connectées à ce sommet
    struct HyperSommet* suivant;     // Suivant dans la liste globale des sommets
} HyperSommet;

// Structure pour représenter une arête (Edge) ou hyperarête
typedef struct HyperArete {
    int poids;                  // Poids de l'arête (non unique)
    HyperSommetNode* sommets;        // Liste des sommets connectés par cette arête
    struct HyperArete* suivant;      // Suivant dans la liste globale des arêtes
} HyperArete;

// Structure pour représenter un maillon d'une liste chaînée d'arêtes
typedef struct HyperAreteNode {
    HyperArete* arete;
    struct HyperAreteNode* suivant;
} HyperAreteNode;

// Structure pour représenter le graphe (hypergraphe)
typedef struct HyperGraphe {
    HyperSommet* listeSommets;       // Tête de la liste des sommets
    HyperArete* listeAretes;         // Tête de la liste des arêtes
} HyperGraphe;

// Prototypes de fonctions

// Créer un nouveau graphe vide
HyperGraphe* creer_hypergraphe();

// Créer et ajouter un sommet au graphe
HyperSommet* ajouter_hypersommet(HyperGraphe* graphe, int id);

// Créer et ajouter une arête au graphe
HyperArete* ajouter_hyperarete(HyperGraphe* graphe, int poids);

// Ajouter un sommet à une arête existante
void ajouter_sommet_a_hyperarete(HyperArete* arete, HyperSommet* sommet);

// Afficher le contenu du graphe
void afficher_hypergraphe(HyperGraphe* graphe);

// Libérer la mémoire du graphe
void liberer_hypergraphe(HyperGraphe* graphe);

// Trouver un sommet par son ID (utilitaire)
HyperSommet* trouver_hypersommet(HyperGraphe* graphe, int id);

#endif
