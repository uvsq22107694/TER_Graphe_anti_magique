#include "antimagic.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int u;
    int v;
} Arete;

// Structure pour stocker le contexte de la récursion
typedef struct {
    Graphe* g;
    Arete* aretes;
    int nb_aretes;
    int* poids_aretes; // Tableau de taille nb_aretes contenant les valeurs 1..M
    bool solution_trouvee;
} Contexte;

void swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Vérifie si l'assignation actuelle des poids donne des sommes distinctes aux sommets
bool verifier_sommes_distinctes(Contexte* ctx) {
    // 1. Appliquer les poids au graphe (matrice d'adjacence)
    // Réinitialiser la matrice d'adjacence ou juste mettre à jour ?
    // On met à jour selon ctx->poids_aretes
    for (int k = 0; k < ctx->nb_aretes; k++) {
        int u = ctx->aretes[k].u;
        int v = ctx->aretes[k].v;
        int poids = ctx->poids_aretes[k];
        ctx->g->matrice_adj[u][v] = poids;
        ctx->g->matrice_adj[v][u] = poids;
    }

    // 2. Calculer les sommes aux sommets
    int n = ctx->g->num_sommets;
    int* sommes = (int*)calloc(n, sizeof(int));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sommes[i] += ctx->g->matrice_adj[i][j];
        }
    }

    // 3. Vérifier l'unicité des sommes
    bool distinct = true;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (sommes[i] == sommes[j]) {
                distinct = false;
                break;
            }
        }
        if (!distinct) break;
    }

    free(sommes);
    return distinct;
}

// Permutation recursive (Heap's algorithm ou simple backtracking)
void permuter(Contexte* ctx, int l, int r) {
    if (ctx->solution_trouvee) return;

    if (l == r) {
        if (verifier_sommes_distinctes(ctx)) {
            ctx->solution_trouvee = true;
        }
    } else {
        for (int i = l; i <= r; i++) {
            swap_int(&ctx->poids_aretes[l], &ctx->poids_aretes[i]);
            permuter(ctx, l + 1, r);
            if (ctx->solution_trouvee) return;
            swap_int(&ctx->poids_aretes[l], &ctx->poids_aretes[i]); // backtrack
        }
    }
}


// Implémentation des algorithmes

// Implémentation de l'algorithme glouton décroissant
void etiquetage_glouton_decroissant(Graphe* g) {
    if (!g) return;

    int n = g->num_sommets;
    
    // 1. Matrice pour suivre les arêtes déjà pondérées
    bool** assigned = (bool**)malloc(n * sizeof(bool*));
    for (int i = 0; i < n; i++) {
        assigned[i] = (bool*)calloc(n, sizeof(bool));
    }

    // 2. Compter le nombre d'arêtes total (M)
    int m = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (g->matrice_adj[i][j] != 0) {
                m++;
            }
        }
    }

    int k = m; // Poids actuel à attribuer
    int aretes_restantes = m;

    // Boucle principale
    while (aretes_restantes > 0) {
        int meilleur_sommet = -1;
        long long max_score = -1;

        // Chercher le sommet avec le score max parmi ceux ayant des arêtes non pondérées
        for (int i = 0; i < n; i++) {
            long long somme_ponderes = 0;
            int count_non_ponderes = 0;

            for (int j = 0; j < n; j++) {
                if (g->matrice_adj[i][j] != 0) { // Il y a une arête
                    if (assigned[i][j]) {
                        somme_ponderes += g->matrice_adj[i][j];
                    } else {
                        count_non_ponderes++;
                    }
                }
            }

            if (count_non_ponderes > 0) {
                // Score = somme déjà pondérée + (k * nombre non pondérées)
                long long score = somme_ponderes + (long long)k * count_non_ponderes;
                
                if (score > max_score) {
                    max_score = score;
                    meilleur_sommet = i;
                }
            }
        }

        if (meilleur_sommet == -1) {
            break;
        }

        // Attribuer les poids aux arêtes incidentes non encore pondérées
        for (int j = 0; j < n; j++) {
            if (g->matrice_adj[meilleur_sommet][j] != 0 && !assigned[meilleur_sommet][j]) {
                g->matrice_adj[meilleur_sommet][j] = k;
                g->matrice_adj[j][meilleur_sommet] = k; // Graphe non orienté
                
                assigned[meilleur_sommet][j] = true;
                assigned[j][meilleur_sommet] = true;
                
                k--;
                aretes_restantes--;
            }
        }
    }

    // Libération de la mémoire
    for (int i = 0; i < n; i++) {
        free(assigned[i]);
    }
    free(assigned);
}

// Helper pour le tri
typedef struct {
    int id;
    int degre;
} SommetDegre;

int comparer_degres(const void* a, const void* b) {
    SommetDegre* sa = (SommetDegre*)a;
    SommetDegre* sb = (SommetDegre*)b;
    return sa->degre - sb->degre;
}

// Implémentation de l'algorithme par degré croissant
void etiquetage_degre_croissant(Graphe* g) {
    if (!g) return;

    int n = g->num_sommets;

    // 1. Calculer les degrés
    SommetDegre* sommets = (SommetDegre*)malloc(n * sizeof(SommetDegre));
    for (int i = 0; i < n; i++) {
        sommets[i].id = i;
        sommets[i].degre = 0;
        for (int j = 0; j < n; j++) {
            if (g->matrice_adj[i][j] != 0) {
                sommets[i].degre++;
            }
        }
    }

    // 2. Trier par degré croissant
    qsort(sommets, n, sizeof(SommetDegre), comparer_degres);

    // 3. Matrice pour suivre les arêtes déjà pondérées
    bool** assigned = (bool**)malloc(n * sizeof(bool*));
    for (int i = 0; i < n; i++) {
        assigned[i] = (bool*)calloc(n, sizeof(bool));
    }

    int k = 1; // Poids actuel à attribuer (commence à 1)
    
    // 4. Parcourir les sommets triés et assigner les poids croissants
    for (int i = 0; i < n; i++) {
        int u = sommets[i].id;
        
        // Parcourir les voisins de u
        // L'ordre des voisins n'est pas spécifié, on prend l'ordre naturel des indices
        for (int v = 0; v < n; v++) {
            if (g->matrice_adj[u][v] != 0 && !assigned[u][v]) {
                g->matrice_adj[u][v] = k;
                g->matrice_adj[v][u] = k;
                assigned[u][v] = true;
                assigned[v][u] = true;
                k++;
            }
        }
    }

    // Libération
    for (int i = 0; i < n; i++) {
        free(assigned[i]);
    }
    free(assigned);
    free(sommets);
}

bool check_antimagic(Graphe* g) {
    // 1. Calculer les sommes aux sommets
    int n = g->num_sommets;
    int* sommes = (int*)calloc(n, sizeof(int));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sommes[i] += g->matrice_adj[i][j];
        }
    }

    // 2. Vérifier l'unicité des sommes
    bool distinct = true;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (sommes[i] == sommes[j]) {
                distinct = false;
                break;
            }
        }
        if (!distinct) break;
    }

    free(sommes);
    return distinct;
}

bool trouver_etiquetage_antimagique(Graphe* g) {
    if (!g) return false;

    // 1. Lister toutes les arêtes
    int n = g->num_sommets;
    int max_aretes = n * (n - 1) / 2;
    Arete* aretes = (Arete*)malloc(max_aretes * sizeof(Arete));
    int m = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (g->matrice_adj[i][j] != 0) {
                aretes[m].u = i;
                aretes[m].v = j;
                m++;
            }
        }
    }

    // 2. Initialiser les poids disponibles : 1 à m
    int* poids = (int*)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++) {
        poids[i] = i + 1;
    }

    Contexte ctx;
    ctx.g = g;
    ctx.aretes = aretes;
    ctx.nb_aretes = m;
    ctx.poids_aretes = poids;
    ctx.solution_trouvee = false;

    printf("Recherche d'un etiquetage anti-magique pour %d sommets et %d aretes...\n", n, m);

    // 3. Étiquetage glouton décroissant
    etiquetage_glouton_decroissant(g);
    if (check_antimagic(g)) {
        ctx.solution_trouvee = true;
        printf("Etiquetage glouton décroissant valide !\n");
    }

    if (!ctx.solution_trouvee) {
        etiquetage_degre_croissant(g);
        if (check_antimagic(g)) {
            ctx.solution_trouvee = true;
            printf("Etiquetage par degré croissant valide !\n");
        }
    }

    if (!ctx.solution_trouvee) {
        permuter(&ctx, 0, m - 1);
    }

    if (ctx.solution_trouvee) {
        printf("Succes ! Etiquetage trouve.\n");
    } else {
        printf("Echec. Aucun etiquetage trouve.\n");
    }

    free(aretes);
    free(poids);

    return ctx.solution_trouvee;
}
