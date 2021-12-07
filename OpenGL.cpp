#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/camera.h"
#include "header/model.h"
#include "header/object.h"
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
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Show FPS
void setTitleFPS(GLFWwindow *window, int nbFrames);

// Đối tượng đang chọn
int idSelected;

// Danh sách model
vector<Model> listModel;

// Danh sách tất cả đối tượng
vector<Object> listObject;

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

    // load models
    // -----------
    listModel.push_back(Model("./models/Board.obj"));
    listModel.push_back(Model("./models/Rook.obj"));
    listModel.push_back(Model("./models/Knight.obj"));
    listModel.push_back(Model("./models/Bishop.obj"));
    listModel.push_back(Model("./models/King.obj"));
    listModel.push_back(Model("./models/Queen.obj"));
    listModel.push_back(Model("./models/Pawn.obj"));
    // Model ourModel("backpack.obj");

    // Set Object
    listObject.push_back(Object(0, listModel[0], true, glm::vec3(0.0f, 0.0f, 0.0f), false, false));

    // Set Chess for Player 1
    // Vị trí khởi đầu
    float baseX = -1.32f, baseY = -.005f, baseZ = 1.32f;
    float rangeObject = 0.38f;  // Khoảng cách giữa các quân cờ (chỉnh theo ý muốn)
    int indexModel;
    Object obj;
    for (int index = 0; index < 8; ++index) {
        // id, model, checkTexture, position, isFirstPlayer, canSelect
        // Id của cờ player1 từ 65 -> 65 + 8
        // Ánh xạ index thành model
        // 0, 1, 2, 3, 4 -> 1, 2, 3, 4, 5
        // 5, 6, 7 => 3, 2, 1
        // 8 - 15 -> 7
        int id = index + 65;
        if (index >= 8)
            indexModel = 6;
        else if (index <= 4)
            indexModel = index + 1;
        else
            indexModel = 8 - index;
        listObject.push_back(Object(
            id, listModel[indexModel], false,
            glm::vec3(baseX + index % 8 * rangeObject, baseY - index / 8 * rangeObject, baseZ),
            false, true));
        // listObject.back().setSelected(true);
    }

    // Set Chess for Player 2
    baseX = 1.34f, baseY = -.005f, baseZ = -1.34f;
    for (int index = 0; index < 8; ++index) {
        // id, model, checkTexture, position, isFirstPlayer, canSelect
        // Id của cờ player1 từ 65 + 8 -> 65 + 16
        int id = index + 65 + 8;
        if (index >= 8)
            indexModel = 6;
        else if (index <= 4)
            indexModel = index + 1;
        else
            indexModel = 8 - index;
        listObject.push_back(Object(
            id, listModel[indexModel], false,
            glm::vec3(baseX - index % 8 * rangeObject, baseY + index / 8 * rangeObject, baseZ),
            true, true));
    }

    // Get framebuffer size for varying window size
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

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

        // Camera inputs
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

        for (auto &object : listObject) object.render(ourShader, stencilShader, lightPos);

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

    idSelected = res;
    std::cout << "Clicked on:" << res << std::endl;
    if (res >= 65) {
        listObject[idSelected - 65 + 1].setSelected(false);
        listObject[res - 65 + 1].setSelected(true);
    }

    unsigned char colorRGBA[4];
    glReadPixels(xx * x_scale, viewport[3] - yy * y_scale, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                 &colorRGBA);
    std::cout << "color R: " << (int)colorRGBA[0] << std::endl;
}

void setTitleFPS(GLFWwindow *window, int nbFrames) {
    std::string title = "Chess 3D - FPS: " + std::to_string(nbFrames);
    glfwSetWindowTitle(window, title.c_str());
}