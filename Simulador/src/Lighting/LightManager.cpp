#include "../Lighting/LightManager.h"

void LightManager::setDirectionalLight(glm::vec3 dir, glm::vec3 color) {
    directionalLightDir = dir;
    directionalLightColor = color;
}

void LightManager::addPointLight(PointLight* light) {
    pointLights.push_back(light);
}

void LightManager::setFlashLight(FlashLight* light) {
    playerFlashLight = light;
}

void LightManager::togglePointLight(int id) {
    for (PointLight* light : pointLights) {
        if (light && light->getId() == id) {
            light->toggle(); // El foco cambia su estado internamente
            std::cout << "[LightManager] Foco con ID " << id
                << " cambiado a: " << (light->getIsOn() ? "ENCENDIDO" : "APAGADO")
                << std::endl;
            return;
        }
    }
    std::cout << "[LightManager] ADVERTENCIA: No se encontro ningun foco registrado con el ID " << id << std::endl;
}

void LightManager::sendLightsToShader(Shader& shader) {
    shader.use();

    // 1. Luz Direccional
    shader.setVec3("dirLight.direction", directionalLightDir);
    shader.setVec3("dirLight.ambient", directionalLightColor * 0.2f);
    shader.setVec3("dirLight.diffuse", directionalLightColor * 0.5f);
    shader.setVec3("dirLight.specular", directionalLightColor * 0.4f);

    // 2. Arreglo de Luces Puntuales
    shader.setInt("nr_point_lights", static_cast<int>(pointLights.size()));
    for (size_t i = 0; i < pointLights.size(); i++) {
        std::string base = "pointLights[" + std::to_string(i) + "].";
        shader.setVec3(base + "position", pointLights[i]->getGlobalPosition()); 
        shader.setVec3(base + "ambient", pointLights[i]->properties.ambient);
        shader.setVec3(base + "diffuse", pointLights[i]->properties.diffuse);
        shader.setVec3(base + "specular", pointLights[i]->properties.specular);
        shader.setFloat(base + "constant", pointLights[i]->constant);
        shader.setFloat(base + "linear", pointLights[i]->linear);
        shader.setFloat(base + "quadratic", pointLights[i]->quadratic);
    }

    // 3. Linterna
    if (playerFlashLight && playerFlashLight->isOn) {
        shader.setVec3("flashLight.position", playerFlashLight->getGlobalPosition());
        shader.setVec3("flashLight.direction", playerFlashLight->direction);
        shader.setVec3("flashLight.ambient", playerFlashLight->properties.ambient);
        shader.setVec3("flashLight.diffuse", playerFlashLight->properties.diffuse);
        shader.setVec3("flashLight.specular", playerFlashLight->properties.specular);
        shader.setFloat("flashLight.constant", playerFlashLight->constant);
        shader.setFloat("flashLight.linear", playerFlashLight->linear);
        shader.setFloat("flashLight.quadratic", playerFlashLight->quadratic);
        shader.setFloat("flashLight.cutOff", playerFlashLight->cutOff);
        shader.setFloat("flashLight.outerCutOff", playerFlashLight->outerCutOff);
    } else {
        shader.setVec3("flashLight.diffuse", glm::vec3(0.0f));
        shader.setVec3("flashLight.specular", glm::vec3(0.0f));
    }
}