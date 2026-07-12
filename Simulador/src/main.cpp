#define STB_IMAGE_IMPLEMENTATION /
#include "Engine/Window.h"
#include "Engine/InputManager.h"
#include "Engine/CollisionManager.h"
#include "Engine/GameLogic.h"

#include "Graphics/shader.h" 
#include "Scene/model.h"
#include "Scene/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Declaración
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); 
InputManager inputManager(&camera);

// Variables para independizar el movimiento de los FPS (Frames Per Second)[cite: 13]
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Declaración de las funciones Callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
    // 1. Inicializamos nuestra clase Window
    Window gameWindow(800, 600, "Simulador - Cargando Modelo y Camara");
    

    // 2. Configuramos los Callbacks del ratón directo en la ventana de GLFW[cite: 13]
    GLFWwindow* rawWindow = gameWindow.getGLFWWindow();
    glfwSetCursorPosCallback(rawWindow, mouse_callback);
    glfwSetScrollCallback(rawWindow, scroll_callback);

    // Capturamos el ratón para que no se salga de la pantalla (Estilo FPS)[cite: 13]
    glfwSetInputMode(rawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Activamos el Z-Buffer para que la mochila no se dibuje al revés[cite: 13]
    glEnable(GL_DEPTH_TEST);

    // 3. Cargamos los shaders y el modelo 3D[cite: 7, 9]
    Shader myShader("Assets/shaders/model.vs", "Assets/shaders/model.fs");
    Model myModel("Assets/models/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);

    // 4. El Game Loop
    while (!gameWindow.shouldClose()) {
        // Lógica de tiempo por frame[cite: 13]
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Fase de Input
        inputManager.processInput(rawWindow, deltaTime);

        // Limpieza de pantalla (Gris oscuro)
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        // Por esto (agregando la máscara de profundidad con una barra vertical '|'):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //[cite: 7]

        // Activamos el shader antes de enviar matrices[cite: 9]
        myShader.use();

        // Configuramos la Matriz de Proyección (El lente de la cámara)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        myShader.setMat4("projection", projection);

        // Configuramos la Matriz de Vista (Dónde está parada la cámara)
        glm::mat4 view = camera.GetViewMatrix();
        myShader.setMat4("view", view);

        // Configuramos la Matriz del Modelo (Posición, rotación, escala en el mundo)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Puedes escalar aquí si la mochila es muy grande
        myShader.setMat4("model", model);

        // ¡Dibujamos!
        myModel.Draw(myShader);

        // Fase de refresco de pantalla
        gameWindow.swapBuffers();
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