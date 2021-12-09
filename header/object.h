#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "model.h"
class Object {
protected:
    int id;
    Model *model;
    bool checkTexture;
    // Tọa độ thế giới
    glm::vec3 position;
    bool isFirstPlayer;
    bool canSelect;
    bool isSelected = false;

    glm::vec3 blueColor = glm::vec3(0.98f, 0.95f, 0.42f);
    glm::vec3 yellowColor = glm::vec3(0.58f, 0.7f, 0.99f);

public:
    Object(){};
    Object(int id, bool canSelect) {
        this->id = id;
        this->canSelect = canSelect;
    }
    Object(int id, Model *model, bool checkTexture, glm::vec3 position, bool isFirstPlayer = false,
           bool canSelect = false)
        : Object(id, canSelect) {
        this->model = model;
        this->checkTexture = checkTexture;
        this->isFirstPlayer = isFirstPlayer;
        this->position = position;
    }
    ~Object() {}

    void setRealPosition(glm::vec3 position) { this->position = position; }
    glm::vec3 getRealPosition() { return this->position; }

    Model getModel() { return *model; }
    int getId() { return id; }
    void setSelected(bool selected) { isSelected = selected; }
    bool getSelected() { return isSelected; }
    bool swapSelected() { return isSelected = !isSelected; }

    // Hàm render cho class Object
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
        ourShader->setVec3("objectColor", isFirstPlayer ? blueColor : yellowColor);

        // màu ánh sáng
        ourShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        // vị trí
        ourShader->setVec3("lightPos", lightPos);
        // Texture
        ourShader->setFloat("Texture", this->checkTexture);

        // draw board as normal, but don't write the board to the stencil buffer, we only care
        // about the chess. We set its mask to 0x00 to not write to the stencil buffer.
        if (this->canSelect) {
            // Gán vùng vẽ vật thể bằng id
            glStencilFunc(GL_ALWAYS, this->id, 0xFF);
            glStencilMask(0xFF);
        }

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, this->position);
        // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
        // it's a bit too big for our scene, so scale it down
        // Xoay nếu là cờ của người chơi 2
        if (this->canSelect && this->isFirstPlayer) {
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        ourShader->setMat4("model", model);
        this->model->Draw(ourShader);

        // if (this->isSelected)
        //     this->renderSelection(stencilShader);
    }
};