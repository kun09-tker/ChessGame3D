#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/camera.h"
#include "header/chess.h"
#include "header/game.h"
#include "header/model.h"
#include "header/object.h"
#include "header/player.h"
#include "header/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "header/stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
// xử lí chọn model
void processSelection(int xx, int yy);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 690;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Colors
glm::vec3 backGround = glm::vec3(0.72f, 0.51f, 1.0f);

// timing
float currentTime = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 2.0f, 2.0f);

// Show FPS
void setTitleFPS(GLFWwindow *window, int nbFrames);

// Đối tượng đã chọn
int idSelected;
// Đối tượng đang chọn
int idSelecting = 0;
// Lượt
int turn = 0;
// Đã chọn cờ
bool piece_chosen = false;
// Đối tượng game
Game game;
Player *player1 = game.getPlayer(1), *player2 = game.getPlayer(2);

// listModel
vector<Model *> listModel;

vector<Chess *> listChessPlayer1, listChessPlayer2;

// Đối tượng bảng
Object *board;

// 4 biến hỗ trợ selection
int window_width = SCR_WIDTH, window_height = SCR_HEIGHT;
int framebuffer_width, framebuffer_height;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw tạo cửa sổ
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bàn cờ", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Bảo GLFW tôi muốn input con chuột & tắt hiển thị con chuột
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load tất cả hàm của glad
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    // stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
    Shader stencilShader("stencil_color.vs", "stencil_color.fs");

    // Enable stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // Get framebuffer size for varying window size
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

    game.GameInit();
    listModel = game.getListModel();
    listChessPlayer1 = game.getPlayer(1)->listChess;
    listChessPlayer2 = game.getPlayer(2)->listChess;
    board = game.getBoard();

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------

    // init Time
    double previousTime = glfwGetTime();
    int frameCount = 0;
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentTime = glfwGetTime();
        frameCount++;
        if (currentTime - previousTime >= 1.0) {
            setTitleFPS(window, frameCount);
            frameCount = 0;
            previousTime = currentTime;
        }

        // input
        // -----
        camera.Inputs(window);

        // render
        // ------
        // glClearColor(0.98f, 1.0f, 0.88f, 1.0f);
        glClearColor(backGround[0], backGround[1], backGround[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // glClearStencil(0);

        //  make sure we don't update the stencil buffer while drawing the board
        glStencilMask(0x00);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // set uniforms
        stencilShader.use();
        stencilShader.setMat4("view", view);
        stencilShader.setMat4("projection", projection);

        // Set view matrix
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Lát thẳng ô cờ
        glm::mat4 model;
        float scale = 0.1885f;
        float distance = 0.377f;  // khoảng cách 2 ô
        for (int index = 0; index < 64; ++index) {
            // gắn id cho ô cờ, bắt đầu từ 1 -> 64
            glStencilFunc(GL_ALWAYS, index + 1, 0xFF);
            glStencilMask(0xFF);

            // render the loaded model
            ourShader.setVec3("objectColor", (index + index / 8) % 2
                                                 ? glm::vec3(1.0f, 1.0f, 1.0f)
                                                 : glm::vec3(0.682f, 0.263f, 0.118f));
            // công thức đổ màu (index + index / 8) % 2 màu trắng, ngược lại đen
            // màu ánh sáng
            ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            // vị trí
            ourShader.setVec3("lightPos", lightPos);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.32f + index % 8 * distance, 0.001f,
                                                    1.317f - index / 8 * distance));
            // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(scale, scale, scale));
            // it's a bit too big for our scene, so scale it down
            ourShader.setMat4("model", model);
            listModel[7]->Draw(ourShader);  // listModel[7] Plate
        }

        // Vẽ bảng
        board->render(ourShader, stencilShader, lightPos);

        // Vẽ cờ cho người 1chơi 1
        for (auto &chess : listChessPlayer1) chess->render(ourShader, stencilShader, lightPos);

        // Vẽ cờ cho người 2chơi 2
        for (auto &chess : listChessPlayer2) chess->render(ourShader, stencilShader, lightPos);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    // update the size of the framebuffer
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    window_width = width;
    window_height = height;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    double x, y;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &x, &y);
        processSelection(x, y);
        // std::cout << "clic à x=" << x << " y=" << y << std::endl;
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void processSelection(int xx, int yy) {
    GLuint res;
    GLint viewport[4];

    // renderSelection();

    float x_scale, y_scale;
    x_scale = (float)framebuffer_width / window_width;
    y_scale = (float)framebuffer_height / window_height;
    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(xx * x_scale, viewport[3] - yy * y_scale, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT,
                 &res);

    int resTemp = res;
    std::cout << "Clicked on:" << res << std::endl;

    if (res == 0) {
        // TODO
        return;
    }

    if (res >= 1 && res <= 65) {
        // std::cout << "Clicked on:" << res << std::endl;
        res--;
        int xLocation = res / 8, yLocation = res % 8;
        // x Là ô chũ, y là ô số

        std::cout << "Ô: ("
                  << "abcdefgh"[yLocation] << ", " << xLocation + 1 << ")\n";
        // Chọn ô xong sẽ quyết định con cờ di chuyển hay không
        //  TẠM KHÓA

        if (piece_chosen) {
            game.tryMovement(idSelected, xLocation, yLocation);
            if (turn % 2 == 0) {
                listChessPlayer1[idSelecting - 66]->Move(xLocation, yLocation);
            } else {
                listChessPlayer2[idSelecting - 66 - 16]->Move(xLocation, yLocation);
            }
            // turn++;
            piece_chosen = false;
        }
    }
    if (res >= 66) {
        idSelecting = res;
        std::cout << "idSelecting = " << idSelecting << " selected: " << idSelected << std::endl;

        // Click piece 2 times then remove selection
        if (idSelecting == idSelected) {
            if (res <= 66 + 15)
                player1->getChessById(idSelected)->setSelected(false);
            else
                player2->getChessById(idSelected)->setSelected(false);
            // idSelecting = 0;
            piece_chosen = false;
            return;
        }

        // Click another piece then remove selection
        if (idSelected >= 66) {
            if (idSelected >= 82)
                player2->getChessById(idSelected)->setSelected(false);
            else
                player1->getChessById(idSelected)->setSelected(false);
        }

        if (res <= 66 + 15)
            player1->getChessById(idSelecting)->setSelected(true);
        else
            player2->getChessById(idSelecting)->setSelected(true);

        if (piece_chosen) {  // condition TODO
            glm::vec2 pos;
            if (res <= 66 + 15)
                pos = player1->getChessById(idSelecting)->getPosition();
            else
                pos = player2->getChessById(idSelecting)->getPosition();
            game.tryMovement(idSelected, pos[0], pos[1]);
        }
        idSelected = idSelecting;
        piece_chosen = true;
    }
}

void setTitleFPS(GLFWwindow *window, int nbFrames) {
    std::string title = "Chess 3D - FPS: " + std::to_string(nbFrames);
    glfwSetWindowTitle(window, title.c_str());
}