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
    int turn = 1;
    Player *player1, *player2;

    Object *board;

    vector<Model *> listModel;

public:
    Game() {
        player1 = new Player(true);
        player2 = new Player(false);
    };

    vector<Model *> getListModel() { return listModel; }

    Object *getBoard() { return board; }

    int getTurn() { return turn; }

    // get player 1 or 2
    Player *getPlayer(int index) { return index == 1 ? player1 : player2; }

    void setSelected(int ChessID, bool state) {
        Chess *chess;
        if (ChessID >= 66) {
            if (ChessID <= 81)
                chess = player1->getChessById(ChessID);
            else
                chess = player2->getChessById(ChessID);
            if (chess != nullptr)
                chess->setSelected(state);
        }
    }

    void swapSelected(int ChessID) {
        Chess *chess;
        if (ChessID >= 66) {
            if (ChessID <= 81)
                chess = player1->getChessById(ChessID);
            else
                chess = player2->getChessById(ChessID);
            if (chess != nullptr)
                chess->swapSelected();
        }
    }

    bool TrueChess(int ChessID) { return turn == 1 ? (ChessID <= 81) : (ChessID > 81); }

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
            else if (index == 4)
                player1->listChess.push_back(new King(id, listModel[4], false, posX, posY, true));
            else if (index == 3)
                player1->listChess.push_back(new Queen(id, listModel[5], false, posX, posY, true));
            else
                player1->listChess.push_back(new Pawn(id, listModel[7], false, posX, posY, true));
        }
        player1->setKing(player1->listChess[4]);
        // for (auto chess : player1->listChess) {
        //     std::cout << "Num of available move: " << chess->getAvailableMovements().size() << "
        //     "
        //               << chess << std::endl;
        // }

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

    bool IsSamePlayer(int ChessID1, int ChessID2) {
        return (ChessID1 <= 81 && ChessID2 <= 81) || (ChessID1 > 81 && ChessID2 > 81);
    }

    bool tryMovement(int IDchess, int posX, int posY) {
        // Kiểm tra phải lượt của player không
        if ((turn == 1 && IDchess > 81) || (turn == 2 && IDchess <= 81))
            return false;

        Player *current = (turn == 1) ? player1 : player2;
        Player *opponent = (turn == 1) ? player2 : player1;
        Chess *current_piece = current->getChessById(IDchess);

        if (current_piece != nullptr) {
            if (current_piece->canMoveTo(posX, posY)) {
                glm::vec2 tempPos = current_piece->getPosition();
                current_piece->moveTo(posX, posY);
                opponent->computeAvailableMovements(opponent->getChess(), current->getChess());
                std::cout << "success move to " << posX << " " << posY << std::endl;
                if (check(current, opponent, current->getKing()->getPosition()).size() == 0) {
                    current_piece->moveTo(tempPos);
                    std::cout << "success move" << std::endl;
                    this->ejectChess(posX, posY);
                    current_piece->Move(posX, posY);
                    changeTurn();
                } else {
                    std::cout << "\tInvalid move, your king is in check!\n";
                    current_piece->moveTo(tempPos);
                    opponent->computeAvailableMovements(opponent->getChess(), current->getChess());
                }
                return true;
            } else
                std::cout << "can't move to " << posX << " " << posY << std::endl;
        }
        return false;
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

    void changeTurn() {
        turn = (turn == 1) ? 2 : 1;
        computeAvailableMovements();

        if (turn == 1) {
            std::vector<Chess *> checkState =
                check(player1, player2, player1->getKing()->getPosition());
            if (checkState.size() > 0) {
                std::cout << "\nPlayer 1, you are checking!" << std::endl;
                // scene->setSelectTex(1);
                if (checkMate(player1, player2, checkState))
                    endGame(2);
            } else {
                // scene->setSelectTex(0);
            }
        } else if (turn == 2) {
            std::vector<Chess *> checkState =
                check(player2, player1, player2->getKing()->getPosition());
            if (checkState.size()) {
                std::cout << "\nPlayer 2, you are checking!" << std::endl;
                // scene->setSelectTex(1);
                if (checkMate(player2, player1, checkState))
                    endGame(1);
            } else {
                // scene->setSelectTex(0);
            }
        }
    }

    void endGame(int winner) {
        // scene->setSelectTex(2);

        Player *looser;
        if (winner == 1) {
            looser = player2;
        } else {
            looser = player1;
        }
        std::cout << "\nThe player " << winner << " is win! ";

        for (unsigned int i = 0; i < looser->getChess().size(); i++) {
            Chess *chess = looser->getChess()[i];
            if (chess->getName() == "King")
                continue;
            // scene->ejectVAO(chess->getVaoID() - 1);
        }
    }

    bool checkMate(Player *player, Player *opponent, std::vector<Chess *> threateningPieces) {
        // Biến xác định checkmate
        bool isInCheckMate = true;
        // Tọa độ của King
        glm::vec2 tempPos = player->getKing()->getPosition();
        glm::vec2 kingPos = tempPos;
        std::vector<int> temp;

        for (unsigned int j = 0; j < player->getKing()->getAvailableMovements().size(); j++) {
            // Gán temp bằng tọa độ của những nước có thể đi tới
            temp = player->getKing()->getAvailableMovements()[j];
            // Thử di chuyển King
            kingPos = glm::vec2(temp[0], temp[1]);
            player->getKing()->moveTo(kingPos[0], kingPos[1]);

            // Tính toán các nước có thể đi của đối thủ
            opponent->computeAvailableMovements(opponent->getChess(), player->getChess());

            // Kiểm tra có check hay không nếu cho King đi thử nước đó
            if (check(player, opponent, kingPos).size() == 0) {
                isInCheckMate = false;
                break;
            }
        }
        // std::cout << "success1 checkmate" << std::endl;
        player->getKing()->moveTo(tempPos[0], tempPos[1]);
        opponent->computeAvailableMovements(opponent->getChess(), player->getChess());

        if (!isInCheckMate)
            return false;

        if (threateningPieces.size() > 1)
            return true;

        glm::vec2 positionToReach = threateningPieces[0]->getPosition();

        for (unsigned int i = 0; i < player->getChess().size(); i++) {
            for (unsigned int j = 0; j < player->getChess()[i]->getAvailableMovements().size();
                 j++) {
                std::vector<int> possibleMovement =
                    player->getChess()[i]->getAvailableMovements()[j];
                if ((possibleMovement[0] == positionToReach[0]) &&
                    (possibleMovement[1] == positionToReach[1]))
                    return false;
            }
        }
        // std::cout << "success2 checkmate" << std::endl;

        kingPos = player->getKing()->getPosition();
        for (unsigned int i = 0; i < player->getChess().size(); i++) {
            // Duyệt qua các con cờ của player
            if (player->getChess()[i]->getName() == "King")
                // Nếu thấy King thì bỏ qua
                continue;

            tempPos = player->getChess()[i]->getPosition();
            // Tọa dộ của cờ đó
            for (unsigned int j = 0; j < player->getChess()[i]->getAvailableMovements().size();
                 j++) {
                // Duyệt qua các nước đi có thể của cờ
                std::vector<int> possibleMovement =
                    player->getChess()[i]->getAvailableMovements()[j];
                player->getChess()[i]->moveTo(possibleMovement);
                // Di chuyển nó đến
                opponent->computeAvailableMovements(opponent->getChess(), player->getChess());
                // cập nhật lại các nước đi có thể của đối thủ
                if (check(player, opponent, kingPos).size() == 0) {
                    // Nếu không check thì không checkmate
                    // std::cout << player->getChess()[i]->getName() << " (" << possibleMovement[0]
                    //           << ":" << possibleMovement[1] << ") \n";
                    player->getChess()[i]->moveTo(tempPos);
                    return false;
                }
            }
            player->getChess()[i]->moveTo(tempPos);
        }
        // std::cout << "success3 checkmate" << std::endl;

        std::cout << "\nCHECK MATE!\n";

        return true;
    }

    void ejectChess(int x, int y) {
        Chess *chess;
        glm::vec2 position = glm::vec2(x, y);

        for (unsigned int i = 0; i < player1->getChess().size(); i++) {
            chess = player1->getChess()[i];
            if (chess->getPosition()[0] == position[0] && chess->getPosition()[1] == position[1]) {
                // scene->ejectVAO(piece->getVaoID() - 1);
                player1->deleteChessAt(i);
                // scene->deleteVAO(piece->getVaoID()-1);
            }
        }
        for (unsigned int j = 0; j < player2->getChess().size(); j++) {
            chess = player2->getChess()[j];
            if (chess->getPosition()[0] == position[0] && chess->getPosition()[1] == position[1]) {
                // scene->ejectVAO(chess->getVaoID() - 1);
                std::cout << "Ejecting chess at " << x << ":" << y << std::endl;
                player2->deleteChessAt(j);
                // scene->deleteVAO(piece->getVaoID()-1);
            }
        }
    }

    void render(Shader *ourShader, Shader *stencilShader, glm::vec3 lightPos) {
        // Vẽ cờ cho người 1chơi 1
        for (auto &chess : player1->listChess) chess->render(ourShader, stencilShader, lightPos);

        // Vẽ cờ cho người 2chơi 2
        for (auto &chess : player2->listChess) chess->render(ourShader, stencilShader, lightPos);
    }
};