#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "PointLight.h"
#include "../Graphics/shader.h"

class LightManager {
private:
    std::vector<PointLight*> pointLights;
    
    // Luz Direccional (El Sol)
    glm::vec3 dirLightDirection;
    glm::vec3 dirLightColor;
    bool hasDirLight;

public:
    LightManager();
    ~LightManager();

    void addPointLight(PointLight* light);
    void setDirectionalLight(glm::vec3 direction, glm::vec3 color);
    
    // Función clave que llama el SceneManager
    void sendLightsToShader(Shader& shader);
};