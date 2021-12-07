#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "object.h"

class Rook : public Object {
private:
public:
    Rook(){};
    ~Rook(){};

    virtual void computeAvailableMovements(std::vector<Object*> owner,
                                           std::vector<Object*> opponent) {
        std::vector<int> pos = std::vector<int>();
        pos.resize(2);
        pos[0] = posX + 1;
        pos[1] = posY;
        bool found = false;
        while (pos[0] < 8 && !found) {
            for (unsigned int i = 0; i < own.size(); i++) {
                if (own[i]->getPosition()[0] == pos[0] && own[i]->getPosition()[1] == pos[1]) {
                    found = true;
                }
            }
            for (unsigned int j = 0; j < opp.size(); j++) {
                if (opp[j]->getPosition()[0] == pos[0] && opp[j]->getPosition()[1] == pos[1]) {
                    availableMovements.push_back(pos);
                    found = true;
                }
            }
            if (!found) {
                availableMovements.push_back(pos);
            }
            pos[0]++;
        }
        pos[0] = posX - 1;
        pos[1] = posY;
        found = false;
        while (pos[0] >= 0 && !found) {
            for (unsigned int i = 0; i < own.size(); i++) {
                if (own[i]->getPosition()[0] == pos[0] && own[i]->getPosition()[1] == pos[1]) {
                    found = true;
                }
            }
            for (unsigned int j = 0; j < opp.size(); j++) {
                if (opp[j]->getPosition()[0] == pos[0] && opp[j]->getPosition()[1] == pos[1]) {
                    availableMovements.push_back(pos);
                    found = true;
                }
            }
            if (!found) {
                availableMovements.push_back(pos);
            }
            pos[0]--;
        }
        pos[0] = posX;
        pos[1] = posY + 1;
        found = false;
        while (pos[1] < 8 && !found) {
            for (unsigned int i = 0; i < own.size(); i++) {
                if (own[i]->getPosition()[0] == pos[0] && own[i]->getPosition()[1] == pos[1]) {
                    found = true;
                }
            }
            for (unsigned int j = 0; j < opp.size(); j++) {
                if (opp[j]->getPosition()[0] == pos[0] && opp[j]->getPosition()[1] == pos[1]) {
                    availableMovements.push_back(pos);
                    found = true;
                }
            }
            if (!found) {
                availableMovements.push_back(pos);
            }
            pos[1]++;
        }
        pos[0] = posX;
        pos[1] = posY - 1;
        found = false;
        while (pos[1] >= 0 && !found) {
            for (unsigned int i = 0; i < own.size(); i++) {
                if (own[i]->getPosition()[0] == pos[0] && own[i]->getPosition()[1] == pos[1]) {
                    found = true;
                }
            }
            for (unsigned int j = 0; j < opp.size(); j++) {
                if (opp[j]->getPosition()[0] == pos[0] && opp[j]->getPosition()[1] == pos[1]) {
                    availableMovements.push_back(pos);
                    found = true;
                }
            }
            if (!found) {
                availableMovements.push_back(pos);
            }
            pos[1]--;
        }
    };
}