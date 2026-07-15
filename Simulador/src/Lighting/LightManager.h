#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Engine/SceneNode.h" 
#include "../Graphics/shader.h" // El compilador necesita saber qué es un Shader aquí

struct LightProperties {
    // Inicializar en 0.0f evita que OpenGL reciba basura y colapse en negro
    glm::vec3 ambient = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f);
    glm::vec3 specular = glm::vec3(0.0f);
};

class PointLight : public SceneNode {
public:
    LightProperties properties;
    float constant;
    float linear;
    float quadratic;
    PointLight() : constant(1.0f), linear(0.09f), quadratic(0.032f) {}
};

class FlashLight : public SceneNode {
public:
    LightProperties properties;
    glm::vec3 direction; 
    float cutOff;      
    float outerCutOff; 
    float constant;
    float linear;
    float quadratic;
    bool isOn; 

    FlashLight() : cutOff(glm::cos(glm::radians(12.5f))), 
                   outerCutOff(glm::cos(glm::radians(17.5f))), 
                   constant(1.0f), linear(0.09f), quadratic(0.032f), isOn(true) {}
};

// === AQUÍ se DECLARA LA CLASE ===
class LightManager {
private:
    std::vector<PointLight*> pointLights;
    FlashLight* playerFlashLight;
    glm::vec3 directionalLightDir; 
    glm::vec3 directionalLightColor;

public:
    LightManager() : playerFlashLight(nullptr) {} // Constructor seguro
    
    // NUEVO: Permite al SceneManager leer las luces para dibujarlas
    const std::vector<PointLight*>& getPointLights() const { return pointLights; }
    void setDirectionalLight(glm::vec3 dir, glm::vec3 color);
    void addPointLight(PointLight* light);
    void setFlashLight(FlashLight* light);
    void togglePointLight(int index, bool state);
    void sendLightsToShader(Shader& shader);
};