#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "SceneNode.h" // Hereda transformaciones de posición/rotación

// Estructura base para las propiedades de la luz de Phong
struct LightProperties {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

// Clase para Focos en el techo (PointLight)
class PointLight : public SceneNode {
public:
    LightProperties properties;
    
    // Parámetros de Atenuación obligatorios
    float constant;
    float linear;
    float quadratic;

    PointLight() : constant(1.0f), linear(0.09f), quadratic(0.032f) {}
};

// Clase para la Linterna (FlashLight / SpotLight)
class FlashLight : public SceneNode {
public:
    LightProperties properties;
    glm::vec3 direction; // Dirección hacia donde apunta la linterna
    
    // Parámetros del cono de luz
    float cutOff;      // Ángulo interno (en coseno)
    float outerCutOff; // Ángulo externo para suavizado (en coseno)
    
    float constant;
    float linear;
    float quadratic;

    bool isOn; // Estado de la linterna (Encendido/Apagado)

    FlashLight() : cutOff(glm::cos(glm::radians(12.5f))), 
                   outerCutOff(glm::cos(glm::radians(17.5f))), 
                   constant(1.0f), linear(0.09f), quadratic(0.032f), isOn(true) {}
};