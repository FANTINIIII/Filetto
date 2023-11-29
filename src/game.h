#ifndef GAME_H
#define GAME_H

struct Game {
    int nodes[24] = {-1};
    bool edges[24*24] = {0};
    int freePawns[2] = {9, 9};
    int playedPawns[2] = {0, 0};
    bool turn = 0;
    bool finished = 0;
};

void initGame(Game* g);
void handleGame(Game* g);

#endif