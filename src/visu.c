#include "visu.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void visualiser_graphe(Graphe* g) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Visualisation Graphe");

    SetTargetFPS(60);

    int n = g->num_sommets;
    Vector2* positions = (Vector2*)malloc(n * sizeof(Vector2));
    
    // Calcul des positions (disposition circulaire)
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    float radius = 200.0f;

    for (int i = 0; i < n; i++) {
        float angle = 2 * PI * i / n;
        positions[i].x = centerX + radius * cosf(angle);
        positions[i].y = centerY + radius * sinf(angle);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dessiner les arêtes
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (g->matrice_adj[i][j] != 0) {
                    DrawLineV(positions[i], positions[j], GRAY);
                    
                    // Afficher le poids de l'arête
                    Vector2 mid = { (positions[i].x + positions[j].x)/2.0f, (positions[i].y + positions[j].y)/2.0f };
                    char buf[16];
                    sprintf(buf, "%d", g->matrice_adj[i][j]);
                    // Petit fond blanc pour la lisibilité
                    DrawCircle((int)mid.x, (int)mid.y, 10, WHITE);
                    DrawText(buf, (int)mid.x - 5, (int)mid.y - 5, 20, DARKGRAY);
                }
            }
        }

        // Dessiner les sommets
        for (int i = 0; i < n; i++) {
            DrawCircleV(positions[i], 25, BLUE);
            
             // Calculer la somme des poids incidents
            int somme = 0;
            for(int k=0; k<n; k++) {
                somme += g->matrice_adj[i][k];
            }

            char buf[16];
            sprintf(buf, "%d", somme);
            DrawText(buf, (int)positions[i].x - 10, (int)positions[i].y - 10, 20, WHITE);
        }

        DrawText("Appuyez sur ESC pour quitter", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    free(positions);
    CloseWindow();
}
