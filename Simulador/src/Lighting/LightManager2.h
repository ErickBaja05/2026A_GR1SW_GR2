#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Engine/SceneNode.h" 
#include "../Graphics/shader.h" // El compilador necesita saber qué es un Shader aquí
#include "../Lighting/PointLight.h"
#include "../Lighting/FlashLight.h"

struct LightProperties {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
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

// === AQUÍ VA LA DECLARACIÓN DE LA CLASE ===
class LightManager {
private:
    std::vector<PointLight*> pointLights;
    FlashLight* playerFlashLight;
    glm::vec3 directionalLightDir; 
    glm::vec3 directionalLightColor;

public:
    LightManager() : playerFlashLight(nullptr) {} // Constructor seguro
    
    void setDirectionalLight(glm::vec3 dir, glm::vec3 color);
    void addPointLight(PointLight* light);
    void setFlashLight(FlashLight* light);
    void togglePointLight(int index, bool state);
    void sendLightsToShader(Shader& shader);
};