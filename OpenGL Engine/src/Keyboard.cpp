#include "Keyboard.h"

struct Direction {
    bool forward = false;
    bool back = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
} dir;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS)
            dir.forward = true;
        else if (action == GLFW_RELEASE)
            dir.forward = false;
    }
    else if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS)
            dir.back = true;
        else if (action == GLFW_RELEASE)
            dir.back = false;
    }
    else if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS)
            dir.left = true;
        else if (action == GLFW_RELEASE)
            dir.left = false;
    }
    else if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS)
            dir.right = true;
        else if (action == GLFW_RELEASE)
            dir.right = false;
    }

    else if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS)
            dir.up = true;
        else if (action == GLFW_RELEASE)
            dir.up = false;
    }
    else if (key == GLFW_KEY_LEFT_SHIFT) {
        if (action == GLFW_PRESS)
            dir.down = true;
        else if (action == GLFW_RELEASE)
            dir.down = false;
    }
}

void processInput(Camera& camera, float deltaTime)
{
    const float cameraSpeed = 16.0f * deltaTime;
    if (dir.forward) {
        camera.translate(cameraSpeed * camera.getFront());
    }
    if (dir.back) {
        camera.translate(-(cameraSpeed * camera.getFront()));
    }
    if (dir.left) {
        camera.translate(-glm::normalize(glm::cross(camera.getFront(), camera.getUp())) * cameraSpeed);
    }
    if (dir.right) {
        camera.translate(-glm::normalize(glm::cross(camera.getFront(), camera.getUp())) * -cameraSpeed);
    }
    if (dir.up) {
        camera.translate(glm::vec3(0, 1.0f, 0) * cameraSpeed);
    }
    if (dir.down) {
        camera.translate(glm::vec3(0, -1.0f, 0) * cameraSpeed);
    }
}