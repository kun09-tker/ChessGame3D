#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "chess.h"

class Pawn : public Chess {
private:
    int d = 0;
    std::string name = "Pawn";

public:
    Pawn(){};
    Pawn(int id, Model *model, bool checkTexture, int posX, int posY, bool isFirstPlayer)
        : Chess(id, model, checkTexture, posX, posY, isFirstPlayer) {
        if (isFirstPlayer) {
            d = 1;
        } else {
            d = -1;
        }
    };

    std::string getName() { return name; }

    virtual void computeAvailableMovements(std::vector<Chess *> &own, std::vector<Chess *> &opp) {
        std::vector<int> pos = std::vector<int>();
        pos.resize(2);
        bool found = false;

        pos[0] = this->posX + d;  // đi lên hoặc lùi 1 ô tùy theo d
        pos[1] = posY;

        // Tìm ô sắp đến đã có cờ chưa
        for (unsigned int i = 0; i < own.size(); i++) {
            if (own[i]->getPosition()[0] == pos[0] && own[i]->getPosition()[1] == pos[1]) {
                found = true;
            }
        }

        // Tìm ô sắp đến đã có cờ chưa
        for (unsigned int j = 0; j < opp.size(); j++) {
            if (opp[j]->getPosition()[0] == pos[0] && opp[j]->getPosition()[1] == pos[1]) {
                found = true;
            }
        }

        // Nếu chưa và vị trí Tốt nằm trong bàn cờ thì
        if (!found && pos[0] >= 0 && pos[0] < 8 && pos[1] >= 0 && pos[1] < 8) {
            availableMovements.push_back(pos);
            if ((d == -1 && posX == 6) || (d == 1 && posX == 1)) {
                // Nếu ở vị trí bắt đầu thì tốt được đi 2 ô
                pos[0] = posX + 2 * d;
                pos[1] = posY;
                // Check xem có tới được hay không
                for (unsigned int i = 0; i < own.size(); i++) {
                    if (own[i]->getPosition()[0] == pos[0] && own[i]->getPosition()[1] == pos[1]) {
                        found = true;
                    }
                }
                for (unsigned int j = 0; j < opp.size(); j++) {
                    if (opp[j]->getPosition()[0] == pos[0] && opp[j]->getPosition()[1] == pos[1]) {
                        found = true;
                    }
                }
                if (!found && pos[0] >= 0 && pos[0] < 8 && pos[1] >= 0 && pos[1] < 8) {
                    availableMovements.push_back(pos);
                }
            }
        }

        // Trường hợp đã tồn tại cờ
        pos[0] = posX + d;
        pos[1] = posY + 1;
        for (unsigned int j = 0; j < opp.size(); j++) {
            if (opp[j]->getPosition()[0] == pos[0] && opp[j]->getPosition()[1] == pos[1]) {
                availableMovements.push_back(pos);
            }
        }

        pos[0] = posX + d;
        pos[1] = posY - 1;
        for (unsigned int j = 0; j < opp.size(); j++) {
            if (opp[j]->getPosition()[0] == pos[0] && opp[j]->getPosition()[1] == pos[1]) {
                availableMovements.push_back(pos);
            }
        }
    };
};