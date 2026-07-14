#pragma once
#include "../Engine/SceneNode.h" // Ajusta la ruta si SceneNode está en otro lado
#include <glm/glm.hpp>

// Estructura para agrupar las propiedades físicas de la luz
struct LightProperties {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    // Valores para atenuación (distancia)
    float constant;
    float linear;
    float quadratic;
};

class PointLight : public SceneNode {
public:
    LightProperties properties;

    PointLight() {
        // Valores por defecto para un foco estándar
        properties.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
        properties.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        properties.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        
        properties.constant = 1.0f;
        properties.linear = 0.09f;
        properties.quadratic = 0.032f;
    }
};