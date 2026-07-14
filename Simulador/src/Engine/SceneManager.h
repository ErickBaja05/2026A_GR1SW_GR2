#pragma once
#include "../Scene/model.h"
#include "../Graphics/shader.h"
#include "../Lighting/LightManager.h"
#include "../Scene/camera.h" // ¡Importante incluir la cámara!

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
    LightManager* lightManager;
    Camera* camera; // Guardamos la cámara para teletransportarla

    std::vector<Model*> neighborhoodProps;
    std::vector<Model*> houseProps;

    Shader* mainShader;
    Shader* skyboxShader;

    unsigned int skyboxVAO, skyboxVBO, cubemapTexture;

    void setupHouseLights();
    unsigned int loadCubemap(std::vector<std::string> faces);
    void setupSkybox();

public:
    // Ahora recibe la cámara
    SceneManager(Shader* main, Shader* sky, LightManager* lm, Camera* cam);
    ~SceneManager();

    void loadNeighborhood();
    void loadHouse();
    void toggleScene(); // Función que usará Josue

    void render(glm::mat4 view, glm::mat4 projection);
    SceneState getCurrentState() { return currentState; }
};