#define STB_IMAGE_IMPLEMENTATION 
#include "Engine/Window.h"
#include "Engine/SceneManager.h"
#include "Lighting/LightManager.h" 
#include "Scene/camera.h"
#include "Input/InputManager.h" 
#include "Input/CollisionManager.h"
#include "Input/GameLogic.h"
#include "Interactable_Objects/Interactable.h"
#include "Interactable_Objects/InteractableManager.h"

// X: 264 (frente a la casa), Y: 4 (altura), Z: 15 (afuera en la calle)
Camera camera(glm::vec3(264.0f, 4.0f, 15.0f)); // Iniciamos un poco más arriba para simular la altura de los ojos
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // 1. Inicializamos la Ventana y OpenGL
    Window gameWindow(1024, 768, "Escape Nocturno - Proyecto Computación Gráfica");

    // Configuraciones globales de OpenGL
    glEnable(GL_DEPTH_TEST);

    // 2. Inicializamos Shaders (Solo el de los modelos, adiós Skybox)
    Shader mainShader("Assets/shaders/model.vs", "Assets/shaders/model.fs");
    Shader lightCubeShader("Assets/shaders/lightcube.vs", "Assets/shaders/lightcube.fs");

    // 3. Inicializamos Managers
    LightManager lightManager;

    // === ¡NUEVO! LUZ DE LUNA PARA PODER VER LA CASA ===
    // === ¡NUEVO! LUZ DE LUNA POTENTE Y EN DIAGONAL ===
    lightManager.setDirectionalLight(glm::vec3(-1.0f, -0.5f, -0.5f), glm::vec3(0.8f, 0.8f, 0.9f));
    // Al instanciar el SceneManager, automáticamente carga la casa (por su nuevo constructor)
    //SceneManager sceneManager(&mainShader, &lightManager, &camera);
    SceneManager sceneManager(&mainShader, &lightCubeShader, &lightManager, &camera);

    /* // ==== CONFIGURACIÓN DE LA LINTERNA DEL JUGADOR ====
    FlashLight playerFlashLight;
    playerFlashLight.properties.ambient = glm::vec3(0.05f); // Luz base muy tenue
    playerFlashLight.properties.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Blanco puro
    playerFlashLight.properties.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Brillo máximo
    playerFlashLight.constant = 1.0f;
    playerFlashLight.linear = 0.045f;
    playerFlashLight.quadratic = 0.0075f;

    // Apertura del cono de luz (Linterna enfocada)
  // Cambia los ángulos de 12.5 y 17.5 a 20 y 30 para tener un foco más amplio
    playerFlashLight.cutOff = glm::cos(glm::radians(20.0f));
    playerFlashLight.outerCutOff = glm::cos(glm::radians(30.0f));
    playerFlashLight.isOn = true;

    // Se la pasamos al LightManager
    lightManager.setFlashLight(&playerFlashLight);
    // ================================================== */

    // 4. Inicializamos Input y Lógica
    InputManager inputManager(&camera);
    CollisionManager collisionManager;
    GameLogic gameLogic(&collisionManager, &inputManager, &sceneManager, &lightManager, &camera);

    // Configuramos los Callbacks de la ventana para el Input
    GLFWwindow* rawWindow = gameWindow.getGLFWWindow();
    glfwSetWindowUserPointer(rawWindow, &inputManager);

    glfwSetCursorPosCallback(rawWindow, [](GLFWwindow* w, double x, double y) {
        static_cast<InputManager*>(glfwGetWindowUserPointer(w))->mouse_callback(w, x, y);
        });

    glfwSetScrollCallback(rawWindow, [](GLFWwindow* w, double x, double y) {
        static_cast<InputManager*>(glfwGetWindowUserPointer(w))->scroll_callback(w, x, y);
        });

    auto interactables = InteractableManager::createHouseInteractables(collisionManager);
    for (auto& obj : interactables) {
        gameLogic.registerInteractable(obj.get());
    }
    // Esta línea es mágica: Oculta el cursor y lo atrapa infinitamente
    glfwSetInputMode(rawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // === GAME LOOP ===
    while (!gameWindow.shouldClose()) {
        // Control del tiempo (DeltaTime)d
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Fase 1: Input y Lógica
        inputManager.processInput(rawWindow, deltaTime);

        gameLogic.update(deltaTime);

        // ==== ACTUALIZAR LA LINTERNA ====
        // Hacemos que la luz emane desde la cámara y apunte hacia donde miramos
        //playerFlashLight.setPosition(camera.Position);
        //playerFlashLight.direction = camera.Front;
        // ================================
        // === ACTUALIZAR LUCES DINÁMICAS EFECTO DISCOTECA ===
        sceneManager.updateLights(currentFrame);

        // Fase 2: Limpieza de pantalla (Fondo negro para la oscuridad)
        glClearColor(1.0f, 0.7f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // FASE 3
        // 1. Preguntamos el tamaño real y actual de la ventana en píxeles
        int width, height;
        glfwGetFramebufferSize(rawWindow, &width, &height);

        // 2. Le ordenamos a OpenGL que su lienzo de dibujo ocupe el 100% de ese tamaño
        glViewport(0, 0, width, height);

        // 3. Calculamos la proyección dividiendo el ancho real para el alto real
        // Esto evita que la casa se vea estirada o aplastada si cambias el tamaño de la ventana
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 5000.0f);
        glm::mat4 view = camera.GetViewMatrix();

        
        // Fase 4: Renderizado Maestro
        sceneManager.render(view, projection, gameLogic.getInteractables());

        // Fase 5: Intercambio de Buffers (VSync) y Eventos
        gameWindow.swapBuffers();
        glfwPollEvents();
    }

    return 0;
}