#include "player.h"

class Game {
private:
    int turn;
    Player *player1, player2;

public:
    Game() { turn = 1; }
};