#pragma once
#include "SceneNode.h"
#include <glm/glm.hpp>

class PointLight : public SceneNode {
protected:
    glm::vec3 color;
    float intensity;
    
    // Parámetros de atenuación de la ecuación de Phong
    float constant;
    float linear;
    float quadratic;

public:
    PointLight(glm::vec3 col = glm::vec3(1.0f), float intens = 1.0f, 
               float constVal = 1.0f, float linVal = 0.09f, float quadVal = 0.032f);

    // Getters y Setters
    glm::vec3 getColor() const { return color; }
    void setColor(const glm::vec3& col) { color = col; }

    float getIntensity() const { return intensity; }
    void setIntensity(float intens) { intensity = intens; }

    float getConstant() const { return constant; }
    float getLinear() const { return linear; }
    float getQuadratic() const { return quadratic; }

    void setAttenuation(float constVal, float linVal, float quadVal);
};