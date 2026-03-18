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

    // 3. Lancer la permutation
    permuter(&ctx, 0, m - 1);

    if (ctx.solution_trouvee) {
        printf("Succes ! Etiquetage trouve.\n");
    } else {
        printf("Echec. Aucun etiquetage trouve.\n");
        // Optionnel : remettre les poids à 1 ou 0 ?
    }

    free(aretes);
    free(poids);

    return ctx.solution_trouvee;
}
