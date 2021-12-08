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
            int posX = index % 8 + 1;
            int posY = index / 8 + 1;
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

        // Set Chess for Player 2
        for (int index = 0; index < 16; ++index) {
            // id, model, checkTexture, position, isFirstPlayer, canSelect
            // Id của cờ player1 từ 66 + 8 -> 66 + 16
            int id = index + 66 + 16;
            int posX = 9 - (index % 8 + 1);
            int posY = 9 - (index / 8 + 1);
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

        // Set Object
        board = new Object(0, listModel[0], true, glm::vec3(0.0f, 0.0f, 0.0f), false, false);
        turn = 1;
    }

    // get player 1 or 2
    Player *getPlayer(int index) { return index == 1 ? player1 : player2; }
};