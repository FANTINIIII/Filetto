#include <stdio.h>
#include "game.h"
#include "raylib.h"

Vector2 points[24] = {
    {200, 100}, {500, 100}, {800, 100}, {800, 400}, {800, 700}, {500, 700}, {200, 700}, {200, 400},
    {300, 200}, {500, 200}, {700, 200}, {700, 400}, {700, 600}, {500, 600}, {300, 600}, {300, 400},
    {400, 300}, {500, 300}, {600, 300}, {600, 400}, {600, 500}, {500, 500}, {400, 500}, {400, 400}
};

Vector3 trisIndices[16] = {
    {0, 1, 2}, {2, 3, 4}, {4, 5, 6}, {6, 7, 0},
    {8, 9, 10}, {10, 11, 12}, {12, 13, 14}, {14, 15, 8},
    {16, 17, 18}, {18, 19, 20}, {20, 21, 22}, {22, 23, 16},
    {1, 9, 17}, {3, 11, 19}, {5, 13, 21}, {7, 15, 23}
};

int trisMap[24*2] = {
    0, 3,
    0, 12,
    0, 1,
    1, 13,
    1, 2,
    2, 14,
    2, 3,
    3, 15,

    4, 7,
    4, 12,
    4, 5,
    5, 13,
    5, 6,
    6, 14,
    6, 7,
    7, 15,

    8, 11,
    8, 12,
    8, 9,
    9, 13,
    9, 10,
    10, 14,
    10, 11,
    11, 15,
};

Color colors[2] = {BLACK, YELLOW};

void initGame(Game* g) {
    for (int i = 0; i < 24; i++) g->nodes[i] = -1;
    g->edges[0*24+1] = 1;
    g->edges[0*24+7] = 1;

    g->edges[1*24+0] = 1;
    g->edges[1*24+2] = 1;
    g->edges[1*24+9] = 1;

    g->edges[2*24+1] = 1;
    g->edges[2*24+3] = 1;

    g->edges[3*24+2] = 1;
    g->edges[3*24+4] = 1;
    g->edges[3*24+11] = 1;

    g->edges[4*24+3] = 1;
    g->edges[4*24+5] = 1;

    g->edges[5*24+4] = 1;
    g->edges[5*24+6] = 1;
    g->edges[5*24+13] = 1;

    g->edges[6*24+5] = 1;
    g->edges[6*24+7] = 1;

    g->edges[7*24+6] = 1;
    g->edges[7*24+0] = 1;
    g->edges[7*24+15] = 1;

    g->edges[8*24+9] = 1;
    g->edges[8*24+15] = 1;

    g->edges[9*24+1] = 1;
    g->edges[9*24+8] = 1;
    g->edges[9*24+10] = 1;
    g->edges[9*24+17] = 1;

    g->edges[10*24+9] = 1;
    g->edges[10*24+11] = 1;

    g->edges[11*24+3] = 1;
    g->edges[11*24+10] = 1;
    g->edges[11*24+12] = 1;
    g->edges[11*24+19] = 1;

    g->edges[12*24+11] = 1;
    g->edges[12*24+13] = 1;

    g->edges[13*24+5] = 1;
    g->edges[13*24+12] = 1;
    g->edges[13*24+14] = 1;
    g->edges[13*24+21] = 1;

    g->edges[14*24+13] = 1;
    g->edges[14*24+15] = 1;

    g->edges[15*24+7] = 1;
    g->edges[15*24+8] = 1;
    g->edges[15*24+14] = 1;
    g->edges[15*24+23] = 1;

    g->edges[16*24+17] = 1;
    g->edges[16*24+23] = 1;

    g->edges[17*24+9] = 1;
    g->edges[17*24+16] = 1;
    g->edges[17*24+18] = 1;

    g->edges[18*24+17] = 1;
    g->edges[18*24+19] = 1;

    g->edges[19*24+11] = 1;
    g->edges[19*24+18] = 1;
    g->edges[19*24+20] = 1;

    g->edges[20*24+19] = 1;
    g->edges[20*24+21] = 1;

    g->edges[21*24+13] = 1;
    g->edges[21*24+20] = 1;
    g->edges[21*24+22] = 1;

    g->edges[22*24+21] = 1;
    g->edges[22*24+23] = 1;

    g->edges[23*24+15] = 1;
    g->edges[23*24+16] = 1;
    g->edges[23*24+22] = 1;
}

int selectedI = -1;
bool rem = 0;

void handleGame(Game* g) {
    // DRAWING BOARD
    DrawRectangleLines(200, 100, 600, 600, BLACK);
    DrawRectangleLines(300, 200, 400, 400, BLACK);
    DrawRectangleLines(400, 300, 200, 200, BLACK);
    DrawLine(500, 100, 500, 300, BLACK);
    DrawLine(500, 500, 500, 700, BLACK);
    DrawLine(200, 400, 400, 400, BLACK);
    DrawLine(600, 400, 800, 400, BLACK);

    Vector2 pos = GetMousePosition();
    SetMouseCursor(0);

    for (int i = 0; i < 24; i++) {
        if ((pos.x - points[i].x) * (pos.x - points[i].x) + (pos.y - points[i].y) * (pos.y - points[i].y) <= 300) {
            // HANDLING MOUSE CLICKS
            if (IsMouseButtonPressed(0)) {
                if (rem) {
                    if (g->nodes[i] == !g->turn) {
                        g->nodes[i] = -1;
                        g->playedPawns[!g->turn]--;
                        rem = 0;
                        if (g->playedPawns[!g->turn] == 0) g->finished = 1;
                        g->turn = !g->turn;
                    }
                }
                else if (g->freePawns[g->turn] > 0) { // DEVO PIAZZARE UNA PEDINA
                    if (g->nodes[i] == -1) { // PIAZZO LA PEDINA
                        g->freePawns[g->turn]--;
                        g->playedPawns[g->turn]++;
                        g->nodes[i] = g->turn;
                        if (g->nodes[int(trisIndices[trisMap[i*2]].x)] == g->turn &&
                            g->nodes[int(trisIndices[trisMap[i*2]].y)] == g->turn &&
                            g->nodes[int(trisIndices[trisMap[i*2]].z)] == g->turn ||
                            g->nodes[int(trisIndices[trisMap[i*2+1]].x)] == g->turn &&
                            g->nodes[int(trisIndices[trisMap[i*2+1]].y)] == g->turn &&
                            g->nodes[int(trisIndices[trisMap[i*2+1]].z)] == g->turn) {
                            rem = 1;
                        }
                        else {
                            g->turn = !g->turn;
                        }
                    }
                }
                else { // DEVO SPOSTARE UNA PEDINA
                    if (selectedI == -1) { // DEVO SELEZIONARE UNA PEDINA
                        if (g->nodes[i] == g->turn) {
                            selectedI = i;
                        }
                        else {
                            selectedI = -1;
                        }
                    }
                    else { // SPOSTO LA PEDINA
                        if (g->nodes[i] == -1 && (g->playedPawns[g->turn] <= 3 || g->playedPawns[g->turn] > 3 && g->edges[24 * selectedI + i] == 1)) {
                            g->nodes[selectedI] = -1;
                            g->nodes[i] = g->turn;
                            selectedI = -1;
                            if (g->nodes[int(trisIndices[trisMap[i*2]].x)] == g->turn &&
                            g->nodes[int(trisIndices[trisMap[i*2]].y)] == g->turn &&
                            g->nodes[int(trisIndices[trisMap[i*2]].z)] == g->turn ||
                            g->nodes[int(trisIndices[trisMap[i*2+1]].x)] == g->turn &&
                            g->nodes[int(trisIndices[trisMap[i*2+1]].y)] == g->turn &&
                            g->nodes[int(trisIndices[trisMap[i*2+1]].z)] == g->turn) {
                            rem = 1;
                        }
                        else {
                            g->turn = !g->turn;
                        }
                        }
                        else if (g->nodes[i] == g->turn) {
                            selectedI = i;
                        }
                        else {
                            selectedI = -1;
                        }
                    }
                }
            }
            // HANDLING MOUSE HOVER
            if (g->freePawns[g->turn] > 0 && g->nodes[i] == -1 && !rem) {
                DrawCircleV(points[i], 10, colors[g->turn]);
                SetMouseCursor(4);
            }
            else if (!rem && g->nodes[i] == g->turn && selectedI == -1) SetMouseCursor(4);
            else if (!rem && selectedI != -1 && g->edges[24 * selectedI + i] == 1 && g->nodes[i] == -1 || !rem && g->nodes[i] == g->turn) {
                DrawCircleV(points[i], 10, colors[g->turn]);
                SetMouseCursor(4);
            }
            if (rem && g->nodes[i] == !g->turn) {
                DrawCircleV(points[i], 10, colors[g->turn]);
                SetMouseCursor(4);
            }
        }
    }

    // DRAW PLACED PAWNS
    for (int i = 0; i < 24; i++) {
        if (selectedI == i) DrawCircleV(points[i], 11, GREEN);
        if (g->nodes[i] != -1) DrawCircleV(points[i], 10, colors[g->nodes[i]]);
    }
}