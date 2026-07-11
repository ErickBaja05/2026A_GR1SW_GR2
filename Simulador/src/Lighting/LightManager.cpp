#include "LightManager.h"
#include "Shader.h"

class LightManager {
private:
    std::vector<PointLight*> pointLights;
    FlashLight* playerFlashLight;
    glm::vec3 directionalLightDir; // Luz del Sol para el vecindario
    glm::vec3 directionalLightColor;

public:
    void setDirectionalLight(glm::vec3 dir, glm::vec3 color) {
        directionalLightDir = dir;
        directionalLightColor = color;
    }

    void addPointLight(PointLight* light) {
        pointLights.push_back(light);
    }

    void setFlashLight(FlashLight* light) {
        playerFlashLight = light;
    }

    // Método para apagar o encender luces dinámicamente (Pedido por el docente)
    void togglePointLight(int index, bool state) {
        if (index >= 0 && index < pointLights.size()) {
            if (!state) {
                pointLights[index]->properties.diffuse = glm::vec3(0.0f);
                pointLights[index]->properties.specular = glm::vec3(0.0f);
            } else {
                // Restaurar valores originales de iluminación
                pointLights[index]->properties.diffuse = glm::vec3(0.8f);
                pointLights[index]->properties.specular = glm::vec3(1.0f);
            }
        }
    }

    // Envío de arreglos y estructuras al Fragment Shader
    void sendLightsToShader(Shader& shader) {
        shader.use();

        // 1. Enviar Luz Direccional (Sol)
        shader.setVec3("dirLight.direction", directionalLightDir);
        shader.setVec3("dirLight.ambient", directionalLightColor * 0.2f);
        shader.setVec3("dirLight.diffuse", directionalLightColor * 0.5f);
        shader.setVec3("dirLight.specular", directionalLightColor * 0.4f);

        // 2. Enviar Arreglo de Luces Puntuales (Focos del cuarto)
        shader.setInt("nr_point_lights", static_cast<int>(pointLights.size()));
        for (size_t i = 0; i < pointLights.size(); i++) {
            std::string base = "pointLights[" + std::to_string(i) + "].";
            // Se obtiene la posición recalculada global del nodo del grafo de escena
            shader.setVec3(base + "position", pointLights[i]->getGlobalPosition()); 
            shader.setVec3(base + "ambient", pointLights[i]->properties.ambient);
            shader.setVec3(base + "diffuse", pointLights[i]->properties.diffuse);
            shader.setVec3(base + "specular", pointLights[i]->properties.specular);
            shader.setFloat(base + "constant", pointLights[i]->constant);
            shader.setFloat(base + "linear", pointLights[i]->linear);
            shader.setFloat(base + "quadratic", pointLights[i]->quadratic);
        }

        // 3. Enviar Linterna (SpotLight)
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
            // Si está apagada, se envían intensidades en cero
            shader.setVec3("flashLight.diffuse", glm::vec3(0.0f));
            shader.setVec3("flashLight.specular", glm::vec3(0.0f));
        }
    }
};