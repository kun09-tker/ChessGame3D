#include <vector>

#include "bishop.h"
#include "chess.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"

class Player {
private:
    // Danh sách cờ của người chơi
    std::vector<Chess *> listChess;

    // Con trỏ đến quân vua
    King *king;

public:
    Player(){};
    ~Player(){};

    std::vector<Chess *> getListChess() { return listChess; }
};