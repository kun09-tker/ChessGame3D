#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "object.h"

class Chess : public Object {
protected:
    // base position
    float baseX = -1.32f, baseY = -.006f, baseZ = 1.32f;
    float rangeObject = 0.377f;  // Khoảng cách giữa các quân cờ (chỉnh theo ý muốn)

    // Toạ độ trong bàn cờ (tính từ 1, 1)
    int posX, posY;

    // Tọa độ có thể di chuyển đến
    std::vector<std::vector<int> > availableMovements;

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

    void setPosition(int posX, int posY) {
        this->posX = posX;
        this->posY = posY;
        this->position = computeRealPosition();
    }

    glm::vec2 getPosition() { return glm::vec2(posX, posY); }

    // Tính toán các tọa độ có thể đi đến
    virtual void computeAvailableMovements(std::vector<Object *> &owner,
                                           std::vector<Object *> &opponent){};

    glm::vec3 computeRealPosition() {
        return glm::vec3(baseX + (this->posX - 1) * rangeObject, baseY,
                         baseZ - (this->posY - 1) * rangeObject);
    }

    // Tính toán toạ độ thực của quân cờ
    glm::vec3 computeRealPosition(int posX, int posY) {
        this->posX = posX;
        this->posY = posY;
        return glm::vec3(baseX + (posX - 1) * rangeObject, baseY, baseZ - (posY - 1) * rangeObject);
    }

    // Hàm render cho class Chess
    virtual void render(Shader ourShader, Shader stencilShader, glm::vec3 lightPos) {
        /*
       Program là Shader
       model là model dùng để load lên
       checkfirstPlayer để quyết định phe mà chọn màu cờ
       checkTexture dùng để biết file có texture để load cho đúng
       x,y,z là tọa độ
       */

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // thiết lập màu đối tượng
        ourShader.setVec3("objectColor", isFirstPlayer ? yellowColor : blueColor);

        // màu ánh sáng
        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        // vị trí
        ourShader.setVec3("lightPos", lightPos);
        // Texture
        ourShader.setFloat("Texture", this->checkTexture);

        // draw board as normal, but don't write the board to the stencil buffer, we only care
        // about the chess. We set its mask to 0x00 to not write to the stencil buffer.
        // Gán vùng vẽ vật thể bằng id
        glStencilFunc(GL_ALWAYS, this->id, 0xFF);
        glStencilMask(0xFF);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->position);
        // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
        // it's a bit too big for our scene, so scale it down
        // Xoay nếu là cờ của người chơi 2
        if (this->canSelect && !this->isFirstPlayer) {
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        ourShader.setMat4("model", model);
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
            stencilShader.use();

            // render the loaded model
            model = glm::mat4(1.0f);
            model = glm::translate(model, this->position);
            // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(0.0055f, 0.005109f, 0.0055f));
            // it's a bit too big for our scene, so scale it down
            stencilShader.setMat4("model", model);
            this->model->Draw(stencilShader);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glEnable(GL_DEPTH_TEST);
        }
    }
};