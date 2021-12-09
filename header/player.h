#pragma once
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
    // biến xác định có phải player 1 không
    bool isFirstPlayer = false;

    // Con trỏ đến quân vua
    Chess *king;

public:
    Player(){};
    Player(bool isFirstPlayer) { this->isFirstPlayer = isFirstPlayer; }
    ~Player(){};

    // Danh sách cờ của người chơi
    std::vector<Chess *> listChess;

    Chess *getChessById(int Id) {
        for (int i = 0; i < (int)listChess.size(); i++) {
            if (listChess[i]->getId() == Id) {
                return listChess[i];
            }
        }
        return nullptr;
    }

    std::vector<Chess *> getChess() { return listChess; }

    Chess *getKing() { return king; }

    void setKing(Chess *king) { this->king = king; }

    void computeAvailableMovements(std::vector<Chess *> own, std::vector<Chess *> opp) {
        for (unsigned int i = 0; i < listChess.size(); i++) {
            listChess[i]->clearAvailableMovements();
            listChess[i]->computeAvailableMovements(own, opp);
        }
    }

    void deleteChessAt(int index) {
        listChess.erase(listChess.begin() + index);
        std::cout << "size listChess: " << listChess.size() << std::endl;
    }

    // std::vector<Chess *> *getListChess() { return listChess; }
};