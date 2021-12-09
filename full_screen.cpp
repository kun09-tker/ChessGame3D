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
#include <filesystem>
#include <iostream>

#include "header/stb_image.h"
namespace fs = std::filesystem;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
// xử lí chọn model
void processSelection(int xx, int yy);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

// settings
unsigned int SCR_WIDTH = 1200;
unsigned int SCR_HEIGHT = 690;

// camera
Camera camera(glm::vec3(5.0f, 1.1f, 0.0f));
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
// Đã chọn cờ
bool piece_chosen = false;
// Đối tượng game
Game game;
Player *player1 = game.getPlayer(1), *player2 = game.getPlayer(2);

// listModel
vector<Model *> listModel;

// Đối tượng bảng
Object *board;

// 4 biến hỗ trợ selection
int window_width = SCR_WIDTH, window_height = SCR_HEIGHT;
int framebuffer_width, framebuffer_height;

unsigned int loadCubemap(vector<std::string> faces);
unsigned int loadTexture(const char *path);

float skyboxVertices[] = {
    // positions
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

    1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

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
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    SCR_WIDTH = mode->width;
    SCR_HEIGHT = mode->height;
    window_width = mode->width;
    window_height = mode->height;

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bàn cờ", monitor, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);

    glfwSwapInterval(0);
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
    Shader *ourShader = new Shader("1.model_loading.vs", "1.model_loading.fs");
    Shader *stencilShader = new Shader("stencil_color.vs", "stencil_color.fs");
    Shader *skyboxShader = new Shader("skybox.vs", "skybox.fs");

    // Enable stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // Get framebuffer size for varying window size
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

    game.GameInit();
    listModel = game.getListModel();
    board = game.getBoard();

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // load textures
    // -------------

    std::string parentDir = (fs::current_path().fs::path::parent_path()).string();

    vector<std::string> faces{parentDir + "/ChessGame3D/textures/right.jpg",
                              parentDir + "/ChessGame3D/textures/left.jpg",
                              parentDir + "/ChessGame3D/textures/top.jpg",
                              parentDir + "/ChessGame3D/textures/bottom.jpg",
                              parentDir + "/ChessGame3D/textures/front.jpg",
                              parentDir + "/ChessGame3D/textures/back.jpg"};

    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------

    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);

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
        ourShader->setMat4("projection", projection);
        ourShader->setMat4("view", view);

        // set uniforms
        stencilShader->use();
        stencilShader->setMat4("view", view);
        stencilShader->setMat4("projection", projection);

        // Set view matrix
        ourShader->use();
        ourShader->setMat4("projection", projection);
        ourShader->setMat4("view", view);

        // Lát thẳng ô cờ
        glm::mat4 model;
        float scale = 0.1885f;
        float distance = 0.377f;  // khoảng cách 2 ô
        for (int index = 0; index < 64; ++index) {
            // gắn id cho ô cờ, bắt đầu từ 1 -> 64
            glStencilFunc(GL_ALWAYS, index + 1, 0xFF);
            glStencilMask(0xFF);

            // render the loaded model
            ourShader->setVec3("objectColor", (index + index / 8) % 2
                                                  ? glm::vec3(1.0f, 1.0f, 1.0f)
                                                  : glm::vec3(0.682f, 0.263f, 0.118f));
            // công thức đổ màu (index + index / 8) % 2 màu trắng, ngược lại đen
            // màu ánh sáng
            ourShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            // vị trí
            ourShader->setVec3("lightPos", lightPos);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.32f + index % 8 * distance, 0.001f,
                                                    1.317f - index / 8 * distance));
            // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(scale, scale, scale));
            // it's a bit too big for our scene, so scale it down
            ourShader->setMat4("model", model);
            listModel[7]->Draw(ourShader);  // listModel[7] Plate
        }

        // Vẽ bảng
        board->render(ourShader, stencilShader, lightPos);

        // Vẽ cờ
        game.render(ourShader, stencilShader, lightPos);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal
                                 // to depth buffer's content
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilMask(0xFF);
        skyboxShader->use();
        skyboxShader->setMat4("view", view);
        skyboxShader->setMat4("projection", projection);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0, 0, 0));
        trans = glm::scale(trans, glm::vec3(20, 20, 20));
        skyboxShader->setMat4("model", trans);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);  // set depth function back to default

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
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

    // int resTemp = res;
    std::cout << "Clicked on:" << res << std::endl;

    if (res == 0) {
        game.setSelected(idSelected, false);
        piece_chosen = false;
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
            std::cout << "selected: " << idSelected << std::endl;
            piece_chosen = !game.tryMovement(idSelected, xLocation, yLocation);
            // turn++;
        }
    }
    if (res >= 66) {
        idSelecting = res;
        std::cout << piece_chosen << " " << !game.IsSamePlayer(idSelecting, idSelected) << " "
                  << game.TrueChess(idSelected) << " idSelecting = " << idSelecting
                  << " selected: " << idSelected << std::endl;

        // Click piece 2 times then remove selection
        if (idSelecting == idSelected) {
            std::cout << "swap" << std::endl;
            game.swapSelected(idSelecting);
            // idSelecting = 0;
            piece_chosen = !piece_chosen;
            return;
        }

        game.setSelected(idSelecting, true);

        if (piece_chosen && !game.IsSamePlayer(idSelecting, idSelected) &&
            game.TrueChess(idSelected)) {  // condition TODO
            std::cout << "move" << std::endl;
            glm::vec2 pos;
            if (res <= 66 + 15)
                pos = player1->getChessById(idSelecting)->getPosition();
            else
                pos = player2->getChessById(idSelecting)->getPosition();
            piece_chosen = !game.tryMovement(idSelected, pos[0], pos[1]);
            if (!piece_chosen)
                // return để không thay đổi cờ đang chọn
                return;
        } else {
            // Click another piece then remove selection
            std::cout << "remove" << std::endl;
            game.setSelected(idSelected, false);

            piece_chosen = true;
        }

        idSelected = idSelecting;
    }
}

void setTitleFPS(GLFWwindow *window, int nbFrames) {
    std::string title = "Chess 3D - FPS: " + std::to_string(nbFrames);
    glfwSetWindowTitle(window, title.c_str());
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // These are very important to prevent seams
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}