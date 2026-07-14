#define STB_IMAGE_IMPLEMENTATION 
#include "Engine/Window.h"
#include "Engine/SceneManager.h"
#include "Lighting/LightManager.h" 
#include "Scene/camera.h"
#include "Input/InputManager.h" 
#include "Input/CollisionManager.h"
#include "Input/GameLogic.h"

Camera camera(glm::vec3(260.0f, 4.0f, 20.0f)); // Iniciamos un poco más arriba para simular la altura de los ojos
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // 1. Inicializamos la Ventana y OpenGL
    Window gameWindow(1024, 768, "Simulador Interactivo - Arquitectura de Software");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);   // <-- agregar esta línea
    glCullFace(GL_BACK);

    // 2. Inicializamos Shaders
    Shader mainShader("Assets/shaders/model.vs", "Assets/shaders/model.fs");
    Shader skyboxShader("Assets/shaders/skybox.vs", "Assets/shaders/skybox.fs");

    // 3. Inicializamos Managers
    LightManager lightManager;
    SceneManager sceneManager(&mainShader, &skyboxShader, &lightManager, &camera);


    // 4. Inicializamos Input
    InputManager inputManager(&camera);
    CollisionManager collisionManager;
    GameLogic gameLogic(&collisionManager, &inputManager, &sceneManager, &lightManager, &camera);

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

        // Fase 1: Input y Lógica
        inputManager.processInput(rawWindow, deltaTime);

        gameLogic.update();

        // Fase 2: Limpieza de pantalla
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Fase 3: Preparación de la Cámara
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1024.0f / 768.0f, 0.1f, 5000.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // Fase 4: Renderizado Maestro
        sceneManager.render(view, projection);

        // Fase 5: Intercambio de Buffers
        gameWindow.swapBuffers();
        glfwPollEvents();
    }

    return 0;
}