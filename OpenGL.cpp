#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/camera.h"
#include "header/model.h"
#include "header/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "header/stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Colors
float backGround[3] = {0.72f, 0.51f, 1.0f};
float firstPlayer[3] = {0.98f, 0.95f, 0.42f};
float secondPlayer[3] = {0.58f, 0.7f, 0.99f};

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Danh sách model
vector<Model> list_model;

// Hàm load model
void loadModel(Shader &Program, Model &model, bool checkfirstPlayer, float checkTexture, float x, float y, float z)
{
    /*
    Program là Shader
    model là model dùng để load lên
    checkfirstPlayer để quyết định phe mà chọn màu cờ
    checkTexture dùng để biết file có texture để load cho đúng
    x,y,z là tọa độ
    */
    glm::mat4 model1 = glm::mat4(1.0f);
    model1 = glm::translate(model1, glm::vec3(x, y, z));            // translate it down so it's at the center of the scene
    model1 = glm::scale(model1, glm::vec3(0.005f, 0.005f, 0.005f)); // it's a bit too big for our scene, so scale it down

    if (checkfirstPlayer)
    {
        Program.setVec3("objectColor", firstPlayer[0], firstPlayer[1], firstPlayer[2]);
        model1 = glm::rotate(model1, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else
    {
        Program.setVec3("objectColor", secondPlayer[0], secondPlayer[1], secondPlayer[2]);
    }

    Program.setMat4("model", model1);
    Program.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    Program.setVec3("lightPos", lightPos);
    Program.setFloat("Texture", checkTexture);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    Program.setMat4("projection", projection);
    Program.setMat4("view", view);

    // render the loaded model
    model.Draw(Program);
}

int main()
{
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
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // load tất cả hàm của glad
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    // stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------

    // load models
    // -----------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
    list_model.push_back(Model("./models/Board.obj"));
    list_model.push_back(Model("./models/Rook.obj"));
    list_model.push_back(Model("./models/Knight.obj"));
    list_model.push_back(Model("./models/Bishop.obj"));
    list_model.push_back(Model("./models/King.obj"));
    list_model.push_back(Model("./models/Queen.obj"));
    list_model.push_back(Model("./models/Pawn.obj"));
    // Model ourModel("backpack.obj");

    // lighting

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        camera.Inputs(window);

        // render
        // ------
        // glClearColor(0.98f, 1.0f, 0.88f, 1.0f);
        glClearColor(backGround[0], backGround[1], backGround[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // don't forget to enable shader before setting uniforms
        ourShader.use();
        loadModel(ourShader, list_model[0], false, 1.0f, 0.0f, 0.0f, 0.0f);

        // loadModel(ourShader, list_model[1], false, 0.0f, -0.56f, 0.005f, 0.2f);
        // First Player
        loadModel(ourShader, list_model[1], false, 0.0f, -1.32f, 0.005f, 1.32f);
        loadModel(ourShader, list_model[2], false, 0.0f, -0.94f, 0.005f, 1.32f);
        loadModel(ourShader, list_model[3], false, 0.0f, -0.56f, 0.005f, 1.32f);
        loadModel(ourShader, list_model[4], false, 0.0f, -0.18f, 0.005f, 1.32f);
        loadModel(ourShader, list_model[5], false, 0.0f, 0.2f, 0.005f, 1.32f);
        loadModel(ourShader, list_model[3], false, 0.0f, 0.58f, 0.005f, 1.32f);
        loadModel(ourShader, list_model[2], false, 0.0f, 0.96f, 0.005f, 1.32f);
        loadModel(ourShader, list_model[1], false, 0.0f, 1.34f, 0.005f, 1.32f);
        loadModel(ourShader, list_model[6], false, 0.0f, -1.32f, 0.005f, 0.94f);
        loadModel(ourShader, list_model[6], false, 0.0f, -0.94f, 0.005f, 0.94f);
        loadModel(ourShader, list_model[6], false, 0.0f, -0.56f, 0.005f, 0.94f);
        loadModel(ourShader, list_model[6], false, 0.0f, -0.18f, 0.005f, 0.94f);
        loadModel(ourShader, list_model[6], false, 0.0f, 0.2f, 0.005f, 0.94f);
        loadModel(ourShader, list_model[6], false, 0.0f, 0.58f, 0.005f, 0.94f);
        loadModel(ourShader, list_model[6], false, 0.0f, 0.96f, 0.005f, 0.94f);
        loadModel(ourShader, list_model[6], false, 0.0f, 1.34f, 0.005f, 0.94f);

        // Second Player
        loadModel(ourShader, list_model[1], true, 0.0f, -1.32f, 0.005f, -1.34f);
        loadModel(ourShader, list_model[2], true, 0.0f, -0.94f, 0.005f, -1.34f);
        loadModel(ourShader, list_model[3], true, 0.0f, -0.56f, 0.005f, -1.34f);
        loadModel(ourShader, list_model[4], true, 0.0f, -0.18f, 0.005f, -1.34f);
        loadModel(ourShader, list_model[5], true, 0.0f, 0.2f, 0.005f, -1.34f);
        loadModel(ourShader, list_model[3], true, 0.0f, 0.58f, 0.005f, -1.34f);
        loadModel(ourShader, list_model[2], true, 0.0f, 0.96f, 0.005f, -1.34f);
        loadModel(ourShader, list_model[1], true, 0.0f, 1.34f, 0.005f, -1.34f);
        loadModel(ourShader, list_model[6], true, 0.0f, -1.32f, 0.005f, -0.96f);
        loadModel(ourShader, list_model[6], true, 0.0f, -0.94f, 0.005f, -0.96f);
        loadModel(ourShader, list_model[6], true, 0.0f, -0.56f, 0.005f, -0.96f);
        loadModel(ourShader, list_model[6], true, 0.0f, -0.18f, 0.005f, -0.96f);
        loadModel(ourShader, list_model[6], true, 0.0f, 0.2f, 0.005f, -0.96f);
        loadModel(ourShader, list_model[6], true, 0.0f, 0.58f, 0.005f, -0.96f);
        loadModel(ourShader, list_model[6], true, 0.0f, 0.96f, 0.005f, -0.96f);
        loadModel(ourShader, list_model[6], true, 0.0f, 1.34f, 0.005f, -0.96f);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react
// accordingly
// ---------------------------------------------------------------------------------------------------------

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}