#define STB_IMAGE_IMPLEMENTATION /
#include "Engine/Window.h"
#include "Graphics/shader.h" 
#include "Scene/model.h"
#include "Scene/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --- VARIABLES GLOBALES PARA LA CÁMARA ---
// (Requisito de GLFW por ser C-API. Josue lo abstraerá luego)
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); // Iniciamos 5 metros hacia atrás[cite: 10, 12]
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
bool firstMouse = true;

// Variables para independizar el movimiento de los FPS (Frames Per Second)[cite: 13]
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Declaración de las funciones Callback[cite: 13]
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInputCamera(GLFWwindow* window);

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

    glEnable(GL_DEPTH_TEST); //

    // 4. El Game Loop
    while (!gameWindow.shouldClose()) {
        // Lógica de tiempo por frame[cite: 13]
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Fase de Input
        processInputCamera(rawWindow); // Teclado[cite: 13]

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

// --- IMPLEMENTACIÓN DE LOS CALLBACKS DE LA CÁMARA ---

// Procesa el teclado en cada frame[cite: 13]
void processInputCamera(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Procesa el movimiento del ratón para mirar alrededor[cite: 13]
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Invertido porque las coordenadas Y van de abajo hacia arriba

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Procesa la rueda del ratón para el Zoom[cite: 13]
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}