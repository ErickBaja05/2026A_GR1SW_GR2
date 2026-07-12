#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Camera;

class InputManager
{
public:
    InputManager(Camera* camera);

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void processInput(GLFWwindow* window, double deltaTime);

private:
    Camera* camera;
    bool firstMouse;

    float lastX;
    float lastY;

};

#endif