#include "PointLight.h"

PointLight::PointLight(glm::vec3 col, float intens, float constVal, float linVal, float quadVal)
    : SceneNode(), color(col), intensity(intens), constant(constVal), linear(linVal), quadratic(quadVal) {
    // Hereda de SceneNode para que si el nodo padre (ej. la casa) se transforma,
    // la posición acumulada del foco cambie de forma automática.
}

void PointLight::setAttenuation(float constVal, float linVal, float quadVal) {
    constant = constVal;
    linear = linVal;
    quadratic = quadVal;
}