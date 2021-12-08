#include <vector>

#include "bishop.h"
#include "chess.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"

class Player {
protected:
    // biến xác định có rotate cờ hay không
    bool isRotated180 = false;

    // Con trỏ đến quân vua
    King *king;

public:
    Player(){};
    ~Player(){};

    // Danh sách cờ của người chơi
    std::vector<Chess *> listChess;

    // std::vector<Chess *> *getListChess() { return listChess; }
};