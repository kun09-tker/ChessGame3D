#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "model.h"
glm::vec3 blueColor = glm::vec3(0.98f, 0.95f, 0.42f);
glm::vec3 yellowColor = glm::vec3(0.58f, 0.7f, 0.99f);

class Object {
private:
    int id;
    Model *model;
    bool checkTexture;
    glm::vec3 position;
    bool isFirstPlayer;
    bool canSelect;
    bool isSelected = false;

public:
    Object(){};
    Object(int id, bool canSelect) {
        this->id = id;
        this->canSelect = canSelect;
    }
    Object(int id, Model &model, bool checkTexture, glm::vec3 position, bool isFirstPlayer = false,
           bool canSelect = false)
        : Object(id, canSelect) {
        this->model = &model;
        this->checkTexture = checkTexture;
        this->isFirstPlayer = isFirstPlayer;
        this->position = position;
    }
    ~Object() {}

    Model getModel() { return *model; }

    int getId() { return id; }
    void setSelected(bool selected) { isSelected = selected; }
    bool getSelected() { return isSelected; }

    void render(Shader ourShader, Shader stencilShader, glm::mat4 projection, glm::mat4 view,
                glm::vec3 lightPos) {
        /*
        Program là Shader
        model là model dùng để load lên
        checkfirstPlayer để quyết định phe mà chọn màu cờ
        checkTexture dùng để biết file có texture để load cho đúng
        x,y,z là tọa độ
        */

        // set uniforms
        stencilShader.use();
        stencilShader.setMat4("view", view);
        stencilShader.setMat4("projection", projection);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // thiết lập màu đối tượng
        ourShader.setVec3("objectColor", isFirstPlayer ? blueColor : yellowColor);

        // màu ánh sáng
        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        // vị trí
        ourShader.setVec3("lightPos", lightPos);
        // Texture
        ourShader.setFloat("Texture", this->checkTexture);

        // Set view matri/x
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

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
        ourShader.setMat4("model", model);
        this->model->Draw(ourShader);

        if (this->isSelected)
            this->renderSelection(stencilShader);
    }

    // Hàm render
    void renderSelection(Shader stencilShader) {
        // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling
        // stencil writing. Because the stencil buffer is now filled with several 1s. The parts of
        // the buffer that are 1 are not drawn, thus only drawing the objects' size differences,
        // making it look like borders.
        // -----------------------------------------------------------------------------------------------------------------------------
        // Những vật thể có ID khác vật đang chọn sẽ không được tô viền
        // glStencilFunc(GL_EQUAL, this->id, 0xFF);
        glStencilFunc(GL_NOTEQUAL, this->id, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        stencilShader.use();

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
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
};