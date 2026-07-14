#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../Lighting/PointLight.h"
#include "../Lighting/FlashLight.h"
#include "../Graphics/shader.h"

class LightManager {
private:
    std::vector<PointLight*> pointLights;
    std::vector<FlashLight*> flashLights;

    // Representación del Sol (Luz Direccional de afuera)
    glm::vec3 sunDirection;
    glm::vec3 sunColor;
    float sunIntensity;

public:
    LightManager();
    ~LightManager() = default;

    // Métodos para recolectar las luces del cuarto/escena actual
    void registerPointLight(PointLight* light);
    void registerFlashLight(FlashLight* light);
    void clearLights();

    // Configuración del Sol
    void setSun(const glm::vec3& dir, const glm::vec3& col, float intensity);

    // Escanea y envía los datos recolectados de Phong a la GPU
    void sendLightsToShader(Shader& shader);
};