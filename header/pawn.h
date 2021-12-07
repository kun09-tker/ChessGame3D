#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "object.h"

class Pawn : public Object {
private:
public:
    Pawn(){};
    ~Pawn(){};

    virtual void computeAvailableMovements(std::vector<Object*> owner,
                                           std::vector<Object*> opponent) {
        std::vector<int> pos = std::vector<int>();
        pos.resize(2);
        bool found = false;

        pos[0] = this->posX + d;
        pos[1] = posY;
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
            if ((d == -1 && posX == 6) || (d == 1 && posX == 1)) {
                pos[0] = posX + 2 * d;
                pos[1] = posY;
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
}