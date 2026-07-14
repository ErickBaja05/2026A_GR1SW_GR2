#pragma once
#include "PointLight.h"

class FlashLight : public PointLight {
private:
    glm::vec3 direction;
    float cutOff;         // Ángulo de apertura interior (en grados)
    float outerCutOff;    // Ángulo de apertura exterior (para bordes suaves, en grados)

public:
    FlashLight(glm::vec3 col = glm::vec3(1.0f), float intens = 1.0f, 
               float constVal = 1.0f, float linVal = 0.09f, float quadVal = 0.032f,
               glm::vec3 dir = glm::vec3(0.0f, -1.0f, 0.0f), float cutAngle = 12.5f, float outerCutAngle = 15.0f);

    // Getters y Setters
    glm::vec3 getDirection() const { return direction; }
    void setDirection(const glm::vec3& dir) { direction = glm::normalize(dir); }

    // Retornan el coseno de los ángulos para mandarlos directamente optimizados al Shader
    float getCutOffCos() const;
    float getOuterCutOffCos() const;

    void setAngles(float cutAngle, float outerCutAngle);
};