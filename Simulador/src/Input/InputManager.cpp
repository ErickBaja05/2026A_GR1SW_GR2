#include "InputManager.h"
#include "../Scene/camera.h"
#include "../Engine/SceneManager.h"
#include "../Lighting/LightManager.h"

InputManager::InputManager(Camera* camera)
{
    this->camera = camera;

    firstMouse = true;
    lastX = 512.0f;
    lastY = 384.0f;
}
// Teclado
void InputManager::processInput(GLFWwindow* window, double deltaTime)
{
    this->window = window;
    // Teclas de movimiento (W,S,A,D)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
}

bool InputManager::isKeyPressed(int key) const
{
    if (!window) return false;
    return glfwGetKey(window, key) == GLFW_PRESS;
}

// Mouse
void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    camera->ProcessMouseMovement(xoffset, yoffset);
}
// Scroll
void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}
