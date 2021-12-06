#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

#include "camera.h"
#include "light.h"
#include "program.h"
#include "vao.h"

static Program program;
static const glm::vec3 color1(1.0f, 1.0f, 1.0f);
static const glm::vec3 color2(0.2f, 0.2f, 0.2f);
static int framebuffer_width, framebuffer_height;
static int window_width, window_height;
static bool skybox_enabled_demo, shadow_enabled_demo, reflection_enabled_demo;
static bool light_enabled_demo[2];
std::vector<Light> lights;
static GLFWwindow* window;
static double lastTime;
static int nbFrames;
static int nbFramesLastSecond;
// Pour la camera
static int midWindowX, midWindowY;
Camera camera;
glm::mat4 projection_matrix, normal_matrix;
glm::mat4 view_matrix =
    glm::lookAt(camera.getPosition(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#define ZNEAR 10.0f
#define ZFAR 10000.0f
#define FOV 0.785f

// list Object
std::vector<Vao> list_object;

void renderSkybox();
void initOpenGL();
void display();
void renderScene();
void error_callback(int error, const char* description) { fputs(description, stderr); }
void window_size_callback(GLFWwindow* window, int width, int height);
void setPerspective(int width, int height);

int main() {
    // glfw: initialize and configure
    // ------------------------------
    light_enabled_demo[0] = light_enabled_demo[1] = false;

    skybox_enabled_demo = shadow_enabled_demo = reflection_enabled_demo = light_enabled_demo[0] =
        light_enabled_demo[1] = true;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window_height = 400;
    window_width = 800;

    midWindowX = window_width / 2;
    midWindowY = window_height / 2;
    window = glfwCreateWindow(window_width, window_height, "Chess3D", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif
    // Make OpenGL
    initOpenGL();
    camera = Camera(10, 10);

    glfwSwapInterval(0);
    glfwSetWindowSizeCallback(window, window_size_callback);

    lastTime = glfwGetTime();
    nbFrames = 0;
    nbFramesLastSecond = 100;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        display();
        // render
        // ------
        /* computing fps */
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {
            nbFramesLastSecond = nbFrames;

            // Set title
            std::string title = "Chess 3D - FPS: " + std::to_string(nbFrames);
            glfwSetWindowTitle(window, title.c_str());

            nbFrames = 0;
            lastTime += 1.0;
        }
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void renderScene() {
    program.use();

    // set view
    const glm::mat4& inv_view_matrix = glm::inverse(view_matrix);

    glUniformMatrix4fv(glGetUniformLocation(program.getId(), "view_matrix"), 1, GL_FALSE,
                       glm::value_ptr(view_matrix));
    glUniformMatrix4fv(glGetUniformLocation(program.getId(), "inv_view_matrix"), 1, GL_FALSE,
                       glm::value_ptr(inv_view_matrix));

    for (unsigned int i = 0; i < lights.size(); ++i) {
        std::string uniform = "light_enabled[" + std::to_string(i) + "]";
        glUniform1i(glGetUniformLocation(program.getId(), uniform.c_str()), light_enabled_demo[i]);
    }

    glViewport(0, 0, framebuffer_width, framebuffer_height);

    for (auto& object : list_object) {
        // std::cout << "render object";
        glm::mat4 model_matrix = object.getModelMatrix();
        glUniform3fv(glGetUniformLocation(program.getId(), "diffuse_color"), 1,
                     object.getDiffuseColorArray());

        glUniform3fv(glGetUniformLocation(program.getId(), "diffuse_color"), 1,
                     object.getDiffuseColorArray());

        glm::mat4 view_model_matrix = view_matrix * model_matrix;
        glm::mat4 proj_view_model_matrix = projection_matrix * view_model_matrix;

        glUniformMatrix4fv(glGetUniformLocation(program.getId(), "proj_view_model"), 1, GL_FALSE,
                           glm::value_ptr(proj_view_model_matrix));
        glUniformMatrix4fv(glGetUniformLocation(program.getId(), "view_model"), 1, GL_FALSE,
                           glm::value_ptr(view_model_matrix));
        glUniform1i(glGetUniformLocation(program.getId(), "texture_enabled"),
                    object.isTextureEnabled());
        glUniform1i(glGetUniformLocation(program.getId(), "skybox_enabled"), 0);

        const GLfloat* normal_matrix_array =
            glm::value_ptr(glm::transpose(glm::inverse(view_model_matrix)));
        glUniformMatrix4fv(glGetUniformLocation(program.getId(), "normal_matrix"), 1, GL_FALSE,
                           normal_matrix_array);

        glBindVertexArray(object.getId());
        glDrawArrays(GL_TRIANGLES, 0, object.getVertexCount());
    }
    glBindVertexArray(0);
}

void display() {
    // for (unsigned int i = 0; i < scene.getLightCount(); ++i) renderShadow(i);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // renderSkybox();
    renderScene();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    program.init();
    // program_shadows.initForShadowMap();
    // program_selection.initForSelection();

    // Init light
    lights.push_back(Light(glm::vec3(400.f, 400.f, 400.f), glm::vec3(0.2f, 0.2f, 0.2f),
                           glm::vec3(0.5f, 0.5f, 0.5f)));
    lights.push_back(Light(glm::vec3(-400.f, 400.f, -400.f), glm::vec3(0.2f, 0.2f, 0.2f),
                           glm::vec3(0.5f, 0.5f, 0.5f)));

    // Init Model
    Vao bishop;
    bishop = Vao::loadObj("models/fou.obj", color1);
    list_object.push_back(bishop);
    std::cout << "Load model successfully" << std::endl;

    // Init camera & projection_matrix
    projection_matrix = glm::mat4(1.0f);
    camera = Camera(window_width, window_height);

    program.use();

    setPerspective(window_width, window_height);
    glUniformMatrix4fv(glGetUniformLocation(program.getId(), "projection_matrix"), 1, GL_FALSE,
                       glm::value_ptr(projection_matrix));

    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

    for (unsigned int i = 0; i < lights.size(); ++i) {
        const Light& light = lights[i];

        std::string pos = "lights[" + std::to_string(i) + "].position";
        std::string dcolor = "lights[" + std::to_string(i) + "].diffuse_color";
        std::string scolor = "lights[" + std::to_string(i) + "].specular_color";

        glUniform3fv(glGetUniformLocation(program.getId(), pos.c_str()), 1, &light.getPos()[0]);
        glUniform3fv(glGetUniformLocation(program.getId(), dcolor.c_str()), 1,
                     &light.getDiffuseColor()[0]);
        glUniform3fv(glGetUniformLocation(program.getId(), scolor.c_str()), 1,
                     &light.getSpecColor()[0]);
    }
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

    window_width = width;
    window_height = height;

    setPerspective(window_width, window_height);
    glUniformMatrix4fv(glGetUniformLocation(program.getId(), "projection_matrix"), 1, GL_FALSE,
                       glm::value_ptr(projection_matrix));
}

void setPerspective(int width, int height) {
    projection_matrix = glm::mat4(1.0f);
    projection_matrix = glm::perspective(FOV, (float)width / height, ZNEAR, ZFAR);
}