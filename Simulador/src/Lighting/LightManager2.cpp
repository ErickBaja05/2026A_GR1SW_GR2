#include "LightManager.h"
#include <string>

LightManager::LightManager() : hasDirLight(false), dirLightDirection(0.0f), dirLightColor(1.0f) {}

LightManager::~LightManager() {
    // La liberación de memoria de los PointLights dependerá de si el SceneManager
    // los destruye, pero el vector interno se limpia aquí.
    pointLights.clear(); 
}

void LightManager::addPointLight(PointLight* light) {
    pointLights.push_back(light);
}

void LightManager::setDirectionalLight(glm::vec3 direction, glm::vec3 color) {
    dirLightDirection = direction;
    dirLightColor = color;
    hasDirLight = true;
}

void LightManager::sendLightsToShader(Shader& shader) {
    shader.use();

    // 1. Enviar Luz Direccional
    shader.setBool("hasDirLight", hasDirLight);
    if (hasDirLight) {
        shader.setVec3("dirLight.direction", dirLightDirection);
        shader.setVec3("dirLight.ambient", dirLightColor * 0.2f);
        shader.setVec3("dirLight.diffuse", dirLightColor * 0.5f);
        shader.setVec3("dirLight.specular", dirLightColor);
    }

    // 2. Enviar Focos (Point Lights)
    shader.setInt("numPointLights", static_cast<int>(pointLights.size()));
    
    for (size_t i = 0; i < pointLights.size(); i++) {
        std::string number = std::to_string(i);
        
        shader.setVec3("pointLights[" + number + "].position", pointLights[i]->getGlobalPosition());
        shader.setVec3("pointLights[" + number + "].ambient", pointLights[i]->properties.ambient);
        shader.setVec3("pointLights[" + number + "].diffuse", pointLights[i]->properties.diffuse);
        shader.setVec3("pointLights[" + number + "].specular", pointLights[i]->properties.specular);
        shader.setFloat("pointLights[" + number + "].constant", pointLights[i]->properties.constant);
        shader.setFloat("pointLights[" + number + "].linear", pointLights[i]->properties.linear);
        shader.setFloat("pointLights[" + number + "].quadratic", pointLights[i]->properties.quadratic);
    }
}