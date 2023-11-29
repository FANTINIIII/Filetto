#include <stdio.h>
#include "raylib.h"
#include "game.h"

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Filetto");
    SetTargetFPS(60);

    Game game;
    initGame(&game);

    int clickedIndex;

    while (!WindowShouldClose()) {

        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DrawFPS(2, 2);

        if (!game.finished) handleGame(&game);
        else {
            int l = MeasureText("Il gioco è finito", 20);
            DrawText("Il gioco è finito", (screenWidth - l) / 2, (screenHeight - 20) / 2, 20, BLACK);
        }

        EndDrawing();
    }


    return 0;
}