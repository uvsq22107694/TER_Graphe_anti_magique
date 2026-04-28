#include <stdio.h>
#include <stdlib.h>
#include "../include/agrah.h"
#include "../include/biparti.h"
#include "../include/visu.h"
#include "../include/test_batterie.h"

int main() {
    printf("\n=== DÉBUT DES TESTS D'ÉTIQUETAGE ANTIMAGIQUE ===\n");
    
    // lancer_batterie_biparti();
    // lancer_batterie_kparti();
    lancer_batterie_hyper_kparti();
    
    printf("\n=== FIN DES TESTS ===\n");

#if 0
    // --- ANCIEN CODE (INCLUANT LA PARTIE GRAPHIQUE) ---

    // 1. Initialisation des variables pour le graphe simple
    int nb_sommets = 3;
    int nb_aretes = 2; // Graphe chemin simple: v1 - v2 - v3

    // --- TEST GRAPHE ANTI-MAGIQUE SIMPLE ---
    printf("========== TEST GRAPHE ANTI-MAGIQUE SIMPLE ==========\n");
    // 2. Création du tableau global référençant tous les sommets du graphe
    Sommet** tous_les_sommets = (Sommet**)malloc(nb_sommets * sizeof(Sommet*));
    if (tous_les_sommets == NULL) {
        fprintf(stderr, "Erreur d'allocation pour le tableau global des sommets.\n");
        return EXIT_FAILURE;
    }

    tous_les_sommets[0] = creer_sommet(1); // v1
    tous_les_sommets[1] = creer_sommet(2); // v2
    tous_les_sommets[2] = creer_sommet(3); // v3

    // 3. Création du graphe possédant `nb_aretes` (2 ici) et `nb_sommets` (3 ici)
    aGraphe* graphe = creer_graphe(nb_sommets, nb_aretes);
    if (graphe == NULL) {
        return EXIT_FAILURE; // Erreur d'allocation gérée dans la fonction
    }

    // 4. Construction et association des arêtes aux labels
    // Arête 1 (Label 1) : relie V1 et V2
    Sommet* arete1_sommets[2] = { tous_les_sommets[0], tous_les_sommets[1] };
    if (assigner_aretes_au_label(graphe, 0, 2, arete1_sommets) != 0) {
        return EXIT_FAILURE;
    }

    // Arête 2 (Label 2) : relie V2 et V3
    Sommet* arete2_sommets[2] = { tous_les_sommets[1], tous_les_sommets[2] };
    if (assigner_aretes_au_label(graphe, 1, 2, arete2_sommets) != 0) {
        return EXIT_FAILURE;
    }

    // 5. Moteur mathématique : Calcul des sommes
    printf("--- Calcul des sommes d'etiquettes ---\n");
    calculer_sommes_sommets(graphe, tous_les_sommets);

    // Affichage des résultats des sommes pour chaque sommet
    for (int i = 0; i < nb_sommets; i++) {
        printf("Sommet V%d a pour somme : %d\n", tous_les_sommets[i]->id, tous_les_sommets[i]->valeur);
    }

    // 6. Vérification de la propriété antimagique
    printf("\n--- Verification Antimagique ---\n");
    int est_anti_magique = est_antimagique(tous_les_sommets, nb_sommets);

    if (est_anti_magique) {
        printf("Le graphe EST antimagique (toutes les sommes sont uniques) !\n");
    } else {
        printf("Le graphe N'EST PAS antimagique (des doublons de sommes ont ete trouves).\n");
    }

    // Libération mémoire graphe
    liberer_graphe(graphe, tous_les_sommets, nb_sommets);

    // --- TEST LEMME 5.1 MATRICE BIPARTI ---
    printf("\n========== TEST MATRICE BIPARTI (LEMME 5.1) ==========\n");
    int m = 4;
    int n = 5; // Exemple avec m=4, n=5 (2 <= m <= n, n >= 4)
    
    // 1. Appel de la fonction "tout-en-un" pour le graphe biparti
    Sommet** sommets_bipartis = NULL;
    aGraphe* graphe_biparti = construire_graphe_biparti_antimagique(m, n, &sommets_bipartis);
    
    if (graphe_biparti != NULL && sommets_bipartis != NULL) {
        int nb_total_sommets = m + n;
        
        // 2. Moteur mathématique : Calcul des sommes
        printf("--- Calcul des sommes d'etiquettes pour le Graphe Biparti K_{%d,%d} ---\n", m, n);
        calculer_sommes_sommets(graphe_biparti, sommets_bipartis);

        // Affichage des résultats des sommes pour chaque sommet
        for (int i = 0; i < nb_total_sommets; i++) {
            printf("Sommet V%d a pour somme : %d\n", sommets_bipartis[i]->id, sommets_bipartis[i]->valeur);
        }

        // 3. Vérification de la propriété antimagique
        printf("\n--- Verification Antimagique ---\n");
        int est_anti_magique_biparti = est_antimagique(sommets_bipartis, nb_total_sommets);

        if (est_anti_magique_biparti) {
            printf("Le graphe biparti K_{%d,%d} EST antimagique (toutes les sommes sont uniques) !\n", m, n);
        } else {
            printf("Le graphe biparti N'EST PAS antimagique (des doublons de sommes ont ete trouves).\n");
        }

        
        // --- VISUALISATION ---
        printf("\nLancement de la visualisation...\n");
        visualiser_agraphe(graphe_biparti, sommets_bipartis, nb_total_sommets);
        

        // 4. Nettoyage mémoire
        liberer_graphe(graphe_biparti, sommets_bipartis, nb_total_sommets);
    }

    printf("\nNettoyage de la memoire et fin propre.\n");
#endif

    return EXIT_SUCCESS;
}

