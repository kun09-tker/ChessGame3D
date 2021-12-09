#pragma once
#include <unistd.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
unsigned int microsecond = 1000000;
#include "object.h"

class Chess : public Object {
protected:
    // base position
    float baseX = -1.32f, baseY = -.006f, baseZ = 1.32f;
    float rangeObject = 0.377f;  // Khoảng cách giữa các quân cờ (chỉnh theo ý muốn)

    // Toạ độ trong bàn cờ (tính từ 0 - 7, a - h)
    int posX, posY;

    // Tọa độ có thể di chuyển đến
    std::vector<std::vector<int> > availableMovements;
    std::vector<glm::vec3> animations;

public:
    Chess(){};
    Chess(int id, Model *model, bool checkTexture, int posX, int posY, bool isFirstPlayer)
        : Object(id, true) {
        this->posX = posX;
        this->posY = posY;
        this->model = model;
        this->checkTexture = checkTexture;
        this->isFirstPlayer = isFirstPlayer;
        this->position = computeRealPosition();
    }

    ~Chess(){};

    virtual std::string getName() { return "Chess"; }

    void setPosition(int posX, int posY) {
        this->posX = posX;
        this->posY = posY;
        this->position = computeRealPosition();
    }
    void setPos(int posX, int posY) {
        this->posX = posX;
        this->posY = posY;
    }
    glm::vec2 getPosition() { return glm::vec2(posX, posY); }

    // Tính toán các tọa độ có thể đi đến
    virtual void computeAvailableMovements(std::vector<Chess *> &own, std::vector<Chess *> &opp){};

    std::vector<std::vector<int> > getAvailableMovements() { return availableMovements; }

    glm::vec3 computeRealPosition() {
        return glm::vec3(baseX + this->posY * rangeObject, baseY, baseZ - this->posX * rangeObject);
    }

    // Tính toán toạ độ thực của quân cờ
    glm::vec3 computeRealPosition(int posX, int posY) {
        this->posX = posX;
        this->posY = posY;
        return this->computeRealPosition();
    }

    // Hàm render cho class Chess
    virtual void render(Shader *ourShader, Shader *stencilShader, glm::vec3 lightPos) {
        /*
       Program là Shader
       model là model dùng để load lên
       checkfirstPlayer để quyết định phe mà chọn màu cờ
       checkTexture dùng để biết file có texture để load cho đúng
       x,y,z là tọa độ
       */

        // don't forget to enable shader before setting uniforms
        ourShader->use();

        // thiết lập màu đối tượng
        ourShader->setVec3("objectColor", isFirstPlayer ? yellowColor : blueColor);

        // màu ánh sáng
        ourShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        // vị trí
        ourShader->setVec3("lightPos", lightPos);
        // Texture
        ourShader->setFloat("Texture", this->checkTexture);

        // draw board as normal, but don't write the board to the stencil buffer, we only care
        // about the chess. We set its mask to 0x00 to not write to the stencil buffer.
        // Gán vùng vẽ vật thể bằng id
        glStencilFunc(GL_ALWAYS, this->id, 0xFF);
        glStencilMask(0xFF);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->position);
        // Thay đổi vị trí của con cờ giống như di chuyển
        if (this->animations.size() != 0) {
            // std::cout << "Vao animation" << std::endl;
            // usleep(0.05 * microsecond);
            this->position = this->animations[0];
            this->animations.erase(this->animations.begin());
        }
        // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
        // it's a bit too big for our scene, so scale it down
        // Xoay nếu là cờ của người chơi 2
        if (this->canSelect && !this->isFirstPlayer) {
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        ourShader->setMat4("model", model);
        this->model->Draw(ourShader);

        if (this->isSelected) {
            // 2nd. render pass: now draw slightly scaled versions of the objects, this time
            // disabling stencil writing. Because the stencil buffer is now filled with several 1s.
            // The parts of the buffer that are 1 are not drawn, thus only drawing the objects' size
            // differences, making it look like borders.
            // -----------------------------------------------------------------------------------------------------------------------------
            // Những vật thể có ID khác vật đang chọn sẽ không được tô viền
            // glStencilFunc(GL_EQUAL, this->id, 0xFF);
            glStencilFunc(GL_NOTEQUAL, this->id, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);
            stencilShader->use();

            // render the loaded model
            model = glm::mat4(1.0f);
            model = glm::translate(model, this->position);
            // translate it down so it's at the center of the scene
            if (this->canSelect && !this->isFirstPlayer) {
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            model = glm::scale(model, glm::vec3(0.0055f, 0.005109f, 0.0055f));
            // it's a bit too big for our scene, so scale it down
            stencilShader->setMat4("model", model);
            this->model->Draw(stencilShader);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glEnable(GL_DEPTH_TEST);
        }
    }

    bool canMoveTo(int targetX, int targetY) {
        std::cout << "Available Movements: " << availableMovements.size() << std::endl;
        for (unsigned int i = 0; i < availableMovements.size(); i++) {
            if (availableMovements[i][0] == targetX && availableMovements[i][1] == targetY) {
                return true;
            }
        }
        return false;
    }

    void moveTo(int targetX, int targetY) {
        posX = targetX;
        posY = targetY;
    }

    void moveTo(glm::vec2 pos) { moveTo(pos[0], pos[1]); }

    void moveTo(std::vector<int> pos) { moveTo(pos[0], pos[1]); }

    void clearAvailableMovements() { availableMovements.clear(); }
    // Dùng để tạo tọa độ giống như di chuyển vào push vào vector animation
    void Move(int finalX, int finalZ) {
        // Vị trí xuất phát
        int startX = posX;
        int startZ = posY;
        // std::cout << "Start: " << posX << " " << posY << std::endl;
        // std::cout << "Final: " << finalX << " " << finalZ << std::endl;
        float x = this->position.x;
        float y = this->position.y;
        float z = this->position.z;
        // std::cout<<"X: "<<x<<" Y: "<<y<<" Z: "<<z<<endl;
        // step
        float stepZ = abs(finalX - this->posX) * 0.377 / 50;
        float stepX = abs(finalZ - this->posY) * 0.377 / 50;
        //Đích đến
        glm::vec3 target = this->computeRealPosition(finalX, finalZ);
        float X = abs(target[0] - x);
        float Y = y;
        float Z = abs(target[2] - z);
        // std::cout<<"X: "<<X<<" Y: "<<Y<<" Z: "<<Z<<endl;

        // Direction
        int directionZ = 1;
        int directionX = 1;
        float z_temp = z;
        float x_temp = x;

        // Khoảng cách
        //  std::cout <<"z,Z: "<< stepZ << " " << Z << std::endl;
        float a = Z / 2;
        if (finalX == startX) {
            a = X / 2;
        }
        // std::cout << "a: " << a << std::endl;
        if (finalX < startX) {
            directionZ = -1;
            z_temp = 2 * a + z;
        }
        if (finalZ < startZ) {
            directionX = -1;
            x_temp = -2 * a + x;
        }
        // std::cout << x_temp << " " << z_temp << std::endl;
        // std::cout <<"Direct" << directionZ << " X: " << directionX << std::endl;
        while (Z > 0) {
            // std::cout <<"Vao White" << std::endl;
            x += stepX * directionX;
            z -= stepZ * directionZ;
            if (finalX == startX) {
                if (X <= 0) {
                    break;
                }
                X -= stepX;
                Y = y + -(0.5f / pow(a, 2.0)) * pow((x - a - x_temp), 2.0) + 0.5;
            } else {
                Z -= stepZ;
                Y = y + -(0.5f / pow(a, 2.0)) * pow((z + a - z_temp), 2.0) + 0.5;
            }

            if (Y <= 0.005) {
                Y = 0.005;
            }
            this->animations.push_back(glm::vec3(x, Y, z));
            // if(Y<=0.005){
            //     break;
            // }
        }
    }
};