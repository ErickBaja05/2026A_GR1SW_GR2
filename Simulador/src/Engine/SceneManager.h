#pragma once
#include "../Scene/model.h"
#include "../Graphics/shader.h"
#include "../Lighting/LightManager.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

enum class SceneState {
    NEIGHBORHOOD,
    INSIDE_HOUSE
};

class SceneManager {
private:
    SceneState currentState;
    LightManager* lightManager; // Guardamos la referencia a la clase de Jorge

    // Listas dinámicas para soportar múltiples .obj separados
    std::vector<Model*> neighborhoodProps;
    std::vector<Model*> houseProps;

    Shader* mainShader;
    Shader* skyboxShader;

    // Variables de OpenGL para el Skybox
    unsigned int skyboxVAO, skyboxVBO, cubemapTexture;

    // Métodos internos
    void setupHouseLights();
    unsigned int loadCubemap(std::vector<std::string> faces);
    void setupSkybox();

public:
    SceneManager(Shader* main, Shader* sky, LightManager* lm);
    ~SceneManager();

    void loadNeighborhood();
    void loadHouse();
    void render(glm::mat4 view, glm::mat4 projection);

    SceneState getCurrentState() { return currentState; }
};
