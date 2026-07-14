#include "LightManager.h"

LightManager::LightManager() 
    : sunDirection(glm::vec3(-0.2f, -1.0f, -0.3f)), sunColor(glm::vec3(1.0f, 0.95f, 0.8f)), sunIntensity(0.5f) {
}

void LightManager::registerPointLight(PointLight* light) {
    if (light) pointLights.push_back(light);
}

void LightManager::registerFlashLight(FlashLight* light) {
    if (light) flashLights.push_back(light);
}

void LightManager::clearLights() {
    pointLights.clear();
    flashLights.clear();
}

void LightManager::setSun(const glm::vec3& dir, const glm::vec3& col, float intensity) {
    sunDirection = glm::normalize(dir);
    sunColor = col;
    sunIntensity = intensity;
}

void LightManager::sendLightsToShader(Shader& shader) {
    shader.use();

    // 1. Enviar el Sol (Direcional)
    shader.setVec3("dirLight.direction", sunDirection);
    shader.setVec3("dirLight.color", sunColor * sunIntensity);

    // 2. Enviar arreglo de Focos (PointLights)
    int numPoints = static_cast<int>(pointLights.size());
    shader.setInt("nbPointLights", numPoints);

    for (int i = 0; i < numPoints; ++i) {
        std::string base = "pointLights[" + std::to_string(i) + "].";
        
        // Al heredar de SceneNode, se asume que existe un método para obtener 
        // la posición global acumulada tras aplicar matrices de transformación (ej: la casa)
        shader.setVec3(base + "position", pointLights[i]->getGlobalPosition()); 
        shader.setVec3(base + "color", pointLights[i]->getColor() * pointLights[i]->getIntensity());
        shader.setFloat(base + "constant", pointLights[i]->getConstant());
        shader.setFloat(base + "linear", pointLights[i]->getLinear());
        shader.setFloat(base + "quadratic", pointLights[i]->getQuadratic());
    }

    // 3. Enviar arreglo de Linternas (FlashLights)
    int numFlashes = static_cast<int>(flashLights.size());
    shader.setInt("nbFlashLights", numFlashes);

    for (int i = 0; i < numFlashes; ++i) {
        std::string base = "flashLights[" + std::to_string(i) + "].";
        
        shader.setVec3(base + "position", flashLights[i]->getGlobalPosition());
        shader.setVec3(base + "direction", flashLights[i]->getDirection());
        shader.setVec3(base + "color", flashLights[i]->getColor() * flashLights[i]->getIntensity());
        shader.setFloat(base + "constant", flashLights[i]->getConstant());
        shader.setFloat(base + "linear", flashLights[i]->getLinear());
        shader.setFloat(base + "quadratic", flashLights[i]->getQuadratic());
        shader.setFloat(base + "cutOff", flashLights[i]->getCutOffCos());
        shader.setFloat(base + "outerCutOff", flashLights[i]->getOuterCutOffCos());
    }
}