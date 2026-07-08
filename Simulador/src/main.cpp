#include "Engine/Window.h"
#include "Graphics/shader.h" // El header del profesor

int main() {
    // 1. Inicializamos nuestra clase Window (Oculta toda la basura de inicialización de GLFW/GLAD)
    Window gameWindow(800, 600, "Prueba de Arquitectura - Nuestro Primer Triangulo");

    // 2. Cargamos nuestro Shader usando la clase del profesor
 
    Shader myShader("Assets/shaders/triangle.vs", "Assets/shaders/triangle.fs");

    // 3. Geometría plana para la prueba (Un Triángulo)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Izquierda
         0.5f, -0.5f, 0.0f, // Derecha
         0.0f,  0.5f, 0.0f  // Arriba
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 4. El Game Loop
    while (!gameWindow.shouldClose()) {
        // Fase de Input
        gameWindow.processInput();

        // Fase de Renderizado
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Color de fondo (Gris azulado)
        glClear(GL_COLOR_BUFFER_BIT);

        myShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // Dibujamos

        // Fase de refresco de pantalla
        gameWindow.swapBuffers();
    }

    return 0;
}