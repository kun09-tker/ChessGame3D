#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "model.h"
#include "object.h"
#include "pawn.h"
#include "player.h"
#include "queen.h"
#include "rook.h"
#include "shader.h"

class Game {
protected:
    int turn;
    Player *player1, *player2;

    Object *board;

    vector<Model *> listModel;

public:
    Game() {
        player1 = new Player();
        player2 = new Player();
    };

    vector<Model *> getListModel() { return listModel; }

    Object *getBoard() { return board; }

    // get player 1 or 2
    Player *getPlayer(int index) { return index == 1 ? player1 : player2; }

    void GameInit() {
        // load models
        // -----------
        listModel.push_back(new Model("./models/Board.obj"));
        listModel.push_back(new Model("./models/Rook.obj"));
        listModel.push_back(new Model("./models/Knight.obj"));
        listModel.push_back(new Model("./models/Bishop.obj"));
        listModel.push_back(new Model("./models/King.obj"));
        listModel.push_back(new Model("./models/Queen.obj"));
        listModel.push_back(new Model("./models/Pawn.obj"));
        listModel.push_back(new Model("./models/Plate.obj"));

        // Set Chess for Player 1
        // Vị trí khởi đầu
        for (int index = 0; index < 16; ++index) {
            // id, model, checkTexture, position, isFirstPlayer, canSelect
            // Id của cờ player1 từ 66 -> 66 + 8
            // Ánh xạ index thành model
            // 0, 1, 2, 3, 4 -> 1, 2, 3, 4, 5
            // 5, 6, 7 => 3, 2, 1
            // 8 - 15 -> 7
            int id = index + 66;
            int posX = index / 8;
            int posY = index % 8;
            if (index >= 8)
                player1->listChess.push_back(new Pawn(id, listModel[6], false, posX, posY, true));
            else if (index == 0 or index == 7)
                player1->listChess.push_back(new Rook(id, listModel[1], false, posX, posY, true));
            else if (index == 1 or index == 6)
                player1->listChess.push_back(new Knight(id, listModel[2], false, posX, posY, true));
            else if (index == 2 or index == 5)
                player1->listChess.push_back(new Bishop(id, listModel[3], false, posX, posY, true));
            else if (index == 3)
                player1->listChess.push_back(new King(id, listModel[4], false, posX, posY, true));
            else if (index == 4)
                player1->listChess.push_back(new Queen(id, listModel[5], false, posX, posY, true));
            else
                player1->listChess.push_back(new Pawn(id, listModel[7], false, posX, posY, true));
        }
        player1->setKing(player1->listChess[3]);
        for (auto chess : player1->listChess) {
            std::cout << "Num of available move: " << chess->getAvailableMovements().size() << " "
                      << chess << std::endl;
        }

        // Set Chess for Player 2
        for (int index = 0; index < 16; ++index) {
            // id, model, checkTexture, position, isFirstPlayer, canSelect
            // Id của cờ player1 từ 66 + 8 -> 66 + 16
            int id = index + 66 + 16;
            int posX = 7 - (index / 8);
            int posY = 7 - (index % 8);
            if (index >= 8)
                player2->listChess.push_back(new Pawn(id, listModel[6], false, posX, posY, false));
            else if (index == 0 or index == 7)
                player2->listChess.push_back(new Rook(id, listModel[1], false, posX, posY, false));
            else if (index == 1 or index == 6)
                player2->listChess.push_back(
                    new Knight(id, listModel[2], false, posX, posY, false));
            else if (index == 2 or index == 5)
                player2->listChess.push_back(
                    new Bishop(id, listModel[3], false, posX, posY, false));
            else if (index == 3)
                player2->listChess.push_back(new King(id, listModel[4], false, posX, posY, false));
            else if (index == 4)
                player2->listChess.push_back(new Queen(id, listModel[5], false, posX, posY, false));
            else
                player2->listChess.push_back(new Pawn(id, listModel[7], false, posX, posY, false));
        }
        player2->setKing(player2->listChess[3]);

        // Set Object
        board = new Object(0, listModel[0], true, glm::vec3(0.0f, 0.0f, 0.0f), false, false);
        turn = 1;

        computeAvailableMovements();
    }

    void computeAvailableMovements() {
        player1->computeAvailableMovements(player1->getChess(), player2->getChess());
        player2->computeAvailableMovements(player2->getChess(), player1->getChess());
    }

    void tryMovement(int IDchess, int posX, int posY) {
        Player *current = (turn == 1) ? player1 : player2;
        Player *opponent = (turn == 1) ? player2 : player1;
        Chess *current_piece = current->getChessById(IDchess);
        if (current_piece != nullptr) {
            std::cout << "success 1" << std::endl;
            if (current_piece->canMoveTo(posX, posY)) {
                glm::vec2 tempPos = current_piece->getPosition();
                current_piece->moveTo(posX, posY);
                opponent->computeAvailableMovements(opponent->getChess(), current->getChess());
                std::cout << "success 2" << std::endl;
                if (check(current, opponent, current->getKing()->getPosition()).size() == 0) {
                    current_piece->moveTo(tempPos);
                    std::cout << "success 3" << std::endl;
                    // ejectPiece(posX, posY);
                    // board.movePieceTo(current_piece->getVaoID(), posX, posY);
                    // changeTurn();
                } else {
                    std::cout << "\tMouvement non valide, votre roi est en échec !\n";
                    current_piece->moveTo(tempPos);
                    opponent->computeAvailableMovements(opponent->getChess(), current->getChess());
                    std::cout << "success 4" << std::endl;
                }
            }
        }
    }

    std::vector<Chess *> check(Player *player, Player *opponent, glm::vec2 KingPos) {
        std::vector<Chess *> threateningPieces = std::vector<Chess *>(0);

        for (unsigned int i = 0; i < opponent->getChess().size(); i++) {
            for (unsigned int j = 0; j < opponent->getChess()[i]->getAvailableMovements().size();
                 j++) {
                std::vector<int> checkSquare = opponent->getChess()[i]->getAvailableMovements()[j];
                if (checkSquare[0] == KingPos[0] && checkSquare[1] == KingPos[1]) {
                    threateningPieces.push_back(opponent->getChess()[i]);
                    if (threateningPieces.size() >= 2) {
                        return threateningPieces;
                    }
                }
            }
        }
        return threateningPieces;
    }
};