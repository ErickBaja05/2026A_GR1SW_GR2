#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
private:
    GLFWwindow* window;
    int width, height;

public:
    Window(int w, int h, const char* title);
    ~Window();

    bool shouldClose();
    void swapBuffers();
    void processInput();
};