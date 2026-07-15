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
private:
    int id = -1;                                  // NUEVO: ID único del foco
    glm::vec3 originalDiffuse = glm::vec3(0.0f);  // NUEVO: Guarda el color para cuando se vuelva a encender
    glm::vec3 originalSpecular = glm::vec3(0.0f); // NUEVO: Guarda el brillo especula
    bool isOn = true;
public:
    LightProperties properties;
    float constant;
    float linear;
    float quadratic;

    PointLight() : constant(1.0f), linear(0.09f), quadratic(0.032f) {}
    // NUEVO: Métodos para asignar y obtener el ID de esta luz
    void setId(int newId) { id = newId; }
    int getId() const { return id; }
    void toggle() {
        isOn = !isOn;
        if (isOn) {
            if (originalDiffuse != glm::vec3(0.0f)) {
                properties.diffuse = originalDiffuse;
                properties.specular = originalSpecular;
            }
        }
        else {
            if (originalDiffuse == glm::vec3(0.0f)) {
                originalDiffuse = properties.diffuse;
                originalSpecular = properties.specular;
            }
            properties.diffuse = glm::vec3(0.0f);
            properties.specular = glm::vec3(0.0f);
        }
    }
    bool getIsOn() const { return isOn; }
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
    
    // NUEVO: Permite al SceneManager leer las luces para dibujarlas
    const std::vector<PointLight*>& getPointLights() const { return pointLights; }
    void setDirectionalLight(glm::vec3 dir, glm::vec3 color);
    void addPointLight(PointLight* light);
    void setFlashLight(FlashLight* light);
    void togglePointLight(int id);
    void sendLightsToShader(Shader& shader);
};