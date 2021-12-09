#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from
// window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
const float YAW = -180.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const unsigned int width = 900;
const unsigned int height = 900;
float sensitivity = 100.0f;
double mX = width / 2, mY = height / 2;
bool after_mouse_move = false;

// An abstract camera class that processes input and calculates the corresponding Euler Angles,
// Vectors and Matrices for use in OpenGL
class Camera {
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    bool firstClick;
    void Inputs(GLFWwindow* window);
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
          MovementSpeed(SPEED),
          MouseSensitivity(SENSITIVITY),
          Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw,
           float pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
          MovementSpeed(SPEED),
          MouseSensitivity(SENSITIVITY),
          Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

    // processes input received from any keyboard-like input system. Accepts input parameter in the
    // form of camera defined ENUM (to abstract it from windowing systems)
    // void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    //     float velocity = MovementSpeed * deltaTime;
    //     if (direction == FORWARD)
    //         Position += Front * velocity;
    //     if (direction == BACKWARD)
    //         Position -= Front * velocity;
    //     if (direction == LEFT)
    //         Position -= Right * velocity;
    //     if (direction == RIGHT)
    //         Position += Right * velocity;
    // }

    // processes input received from a mouse input system. Expects the offset value in both the x
    // and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical
    // wheel-axis
    void ProcessMouseScroll(float yoffset) {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    // Thiết lập trạng thái camera
    void setMoveCamera(bool state) { isMoveCamera = state; }

    // Lấy trạng thái
    bool getMoveCamera() { return isMoveCamera; }

private:
    // options movecamera
    bool isMoveCamera = true;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(
            Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the
                               // more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
void Camera::Inputs(GLFWwindow* window)
{
    float speed = 0.05f;
    // Handles key inputs
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Position += speed * Front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Position += speed * -glm::normalize(glm::cross(Front, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Position += speed * -Front;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Position += speed * glm::normalize(glm::cross(Front, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Position += speed * Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        Position += speed * -Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = 0.4f;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        speed = 0.1f;
    }

    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if (!after_mouse_move) {
            glfwGetCursorPos(window, &mX, &mY);
            after_mouse_move = true;
        }
        // Prevents camera from jumping on the first click
        if (firstClick) {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of
        // the screen and then "transforms" them into degrees
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        // Calculates upcoming vertical change in the Front
        glm::vec3 newFront =
            glm::rotate(Front, glm::radians(-rotX), glm::normalize(glm::cross(Front, Up)));

        // Decides whether or not the next vertical Front is legal or not
        if (abs(glm::angle(newFront, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
            Front = newFront;
        }

        // Rotates the Front left and right
        Front = glm::rotate(Front, glm::radians(-rotY), Up);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around

        glfwSetCursorPos(window, (width / 2), (height / 2));
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        if (after_mouse_move) {
            glfwSetCursorPos(window, mX, mY);
            after_mouse_move = false;
        }
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }
}
#endif