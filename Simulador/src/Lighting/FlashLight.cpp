#include "FlashLight.h"

FlashLight::FlashLight(glm::vec3 col, float intens, float constVal, float linVal, float quadVal,
                       glm::vec3 dir, float cutAngle, float outerCutAngle)
    : PointLight(col, intens, constVal, linVal, quadVal), direction(glm::normalize(dir)), 
      cutOff(cutAngle), outerCutOff(outerAngle) {
}

float FlashLight::getCutOffCos() const {
    return glm::cos(glm::radians(cutOff));
}

float FlashLight::getOuterCutOffCos() const {
    return glm::cos(glm::radians(outerCutOff));
}

void FlashLight::setAngles(float cutAngle, float outerCutAngle) {
    cutOff = cutAngle;
    outerCutOff = outerCutAngle;
}