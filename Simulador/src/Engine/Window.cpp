#include "Window.h"

Window::Window(int w, int h, const char* title, bool fullscreen) : width(w), height(h) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = NULL;
    if (fullscreen) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        width = mode->width;
        height = mode->height;
    }

    window = glfwCreateWindow(width, height, title, monitor, NULL);
    if (window == NULL) {
        std::cout << "Fallo al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // GLAD debe inicializarse justo después de crear el contexto de la ventana
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Fallo al inicializar GLAD" << std::endl;
    }
    glViewport(0, 0, width, height);
}

Window::~Window() {
    glfwTerminate(); // Limpiamos los recursos al cerrar
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::processInput() {
    // Si presionan ESCAPE, le decimos a la ventana que debe cerrarse
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}