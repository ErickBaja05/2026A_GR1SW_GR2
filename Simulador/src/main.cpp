#define STB_IMAGE_IMPLEMENTATION 
#include "Engine/Window.h"
#include "Engine/SceneManager.h"
#include "Graphics/LightManager.h" 
#include "Scene/camera.h"

// JOSUE: Tu clase ahora debe incluir esto en su .h
#include "Input/InputManager.h" 

Camera camera(glm::vec3(0.0f, 2.0f, 5.0f)); // Iniciamos un poco más arriba para simular la altura de los ojos
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // 1. Inicializamos la Ventana y OpenGL
    Window gameWindow(1024, 768, "Simulador Interactivo - Arquitectura de Software");
    glEnable(GL_DEPTH_TEST);

    // 2. Inicializamos Shaders (Jorge)
    Shader mainShader("Assets/shaders/model.vs", "Assets/shaders/model.fs");
    Shader skyboxShader("Assets/shaders/skybox.vs", "Assets/shaders/skybox.fs");

    // 3. Inicializamos Managers (Tú y Jorge)
    LightManager lightManager;
    SceneManager sceneManager(&mainShader, &skyboxShader, &lightManager);

    // 4. Inicializamos Input (Josue)
    // NOTA PARA JOSUE: Tienes que actualizar tu constructor para aceptar sm y lm
    InputManager inputManager(&camera, &sceneManager, &lightManager);

    // Configuramos los Callbacks de la ventana (Redirigidos al InputManager de Josue)
    GLFWwindow* rawWindow = gameWindow.getGLFWWindow();
    glfwSetWindowUserPointer(rawWindow, &inputManager);

    glfwSetCursorPosCallback(rawWindow, [](GLFWwindow* w, double x, double y) {
        static_cast<InputManager*>(glfwGetWindowUserPointer(w))->mouse_callback(w, x, y);
        });
    glfwSetScrollCallback(rawWindow, [](GLFWwindow* w, double x, double y) {
        static_cast<InputManager*>(glfwGetWindowUserPointer(w))->scroll_callback(w, x, y);
        });
    glfwSetInputMode(rawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 5. Estado Inicial
    sceneManager.loadNeighborhood();

    // === GAME LOOP ===
    while (!gameWindow.shouldClose()) {
        // Control del tiempo
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Fase 1: Input y Lógica (Josue)[cite: 5]
        inputManager.processInput(rawWindow, deltaTime);

        // Fase 2: Limpieza de pantalla
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Fase 3: Preparación de la Cámara
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1024.0f / 768.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // Fase 4: Renderizado Maestro (Tu SceneManager)
        sceneManager.render(view, projection);

        // Fase 5: Intercambio de Buffers
        gameWindow.swapBuffers();
        glfwPollEvents();
    }

    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    inputManager.mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    inputManager.scroll_callback(window, xoffset, yoffset);
}