#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Forward declarations para evitar referencias circulares
class Camera;
class SceneManager;
class LightManager;

class InputManager {
public:
    // Ahora recibe los tres componentes vitales
   
    InputManager(Camera* camera, SceneManager* sceneManager, LightManager* lightManager);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void processInput(GLFWwindow* window, double deltaTime);

private:
    Camera* camera;
    SceneManager* sceneManager;
    LightManager* lightManager;
    
    bool firstMouse;
    float lastX;
    float lastY;
};
#endif