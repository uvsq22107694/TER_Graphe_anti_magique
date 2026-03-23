#include "visu.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void visualiser_agraphe(aGraphe* g, Sommet** sommets, int nb_sommets) {
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Visualisation aGraphe (Anti-Magique)");
    SetTargetFPS(60);

    Vector2* positions = (Vector2*)malloc(nb_sommets * sizeof(Vector2));
    
    // Calcul des positions (disposition en cercle)
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    float radius = 280.0f;

    for (int i = 0; i < nb_sommets; i++) {
        float angle = 2 * PI * i / nb_sommets;
        positions[i].x = centerX + radius * cosf(angle);
        positions[i].y = centerY + radius * sinf(angle);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dessiner les arêtes et leurs poids
        for (int i = 0; i < g->n; i++) {
            int poids = i + 1; // Le label commence à 1
            
            // Gérer toutes les connexions pour chaque hyper-arête
            // Parcourir toutes les paires de sommets possibles connectées à cette arête
            for (int j = 0; j < g->nb_sommets - 1; j++) {
                if (g->matrice_incidence[i][j] == 1) {
                    for (int k = j + 1; k < g->nb_sommets; k++) {
                        if (g->matrice_incidence[i][k] == 1) {
                            // On sait que les sommets connectés sont aux indices j et k dans le tableau 'sommets'
                            // car on a assumé: indice_matrice = id - 1, et le tableau 'sommets' contient tous les sommets
                            // dans le bon ordre ou du moins on peut utiliser la recherche pour être sûr
                            int idx1 = -1, idx2 = -1;
                            for (int s = 0; s < nb_sommets; s++) {
                                if (sommets[s]->id - 1 == j) idx1 = s;
                                if (sommets[s]->id - 1 == k) idx2 = s;
                            }
                            
                            if (idx1 != -1 && idx2 != -1) {
                                DrawLineV(positions[idx1], positions[idx2], GRAY);
                                
                                // Affichage du poids de l'arête (au centre de l'arête)
                                Vector2 p1 = positions[idx1];
                                Vector2 p2 = positions[idx2];
                                
                                // Décaler si i pair ou impair pour éviter trop de chevauchements
                                float t = 0.5f; 
                                if (i % 3 == 0) t = 0.4f;
                                else if (i % 3 == 1) t = 0.6f;

                                Vector2 labelPos = { p1.x + (p2.x - p1.x) * t, p1.y + (p2.y - p1.y) * t };
                                char buf[16];
                                sprintf(buf, "%d", poids);
                                
                                int textWidth = MeasureText(buf, 20);
                                int bgRadius = (textWidth > 16) ? (textWidth / 2 + 5) : 14;

                                DrawCircle((int)labelPos.x, (int)labelPos.y, (float)bgRadius, LIGHTGRAY);
                                DrawText(buf, (int)labelPos.x - textWidth / 2, (int)labelPos.y - 10, 20, BLACK);
                            }
                        }
                    }
                }
            }
        }

        // Dessiner les sommets et leurs valeurs (sommes)
        for (int i = 0; i < nb_sommets; i++) {
            DrawCircleV(positions[i], 25, BLUE);
            
            // Afficher le nom du sommet (ex: V1)
            char nom[16];
            sprintf(nom, "V%d", sommets[i]->id);
            DrawText(nom, (int)positions[i].x - 20, (int)positions[i].y - 45, 18, DARKBLUE);
            
            // L'étiquette au centre du sommet correspond ici à sa valeur calculée (la somme)
            char buf[16];
            sprintf(buf, "%d", sommets[i]->valeur);
            int textWidth = MeasureText(buf, 20);
            DrawText(buf, (int)positions[i].x - textWidth / 2, (int)positions[i].y - 10, 20, WHITE);
        }

        DrawText("Appuyez sur ESC pour quitter", 10, 10, 20, DARKGRAY);
        DrawText("Noeuds BLEUS : Sommets avec la somme de leurs aretes", 10, 40, 18, BLUE);
        DrawText("Cercles GRIS : Etiquettes (labels) des aretes", 10, 65, 18, BLACK);

        EndDrawing();
    }

    free(positions);
    CloseWindow();
}
