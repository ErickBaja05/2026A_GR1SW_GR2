#pragma once
#include <vector>
#include <string>
#include "../Graphics/shader.h"
#include "../Scene/model.h"
#include "../Scene/camera.h"
#include "../Lighting/LightManager.h"

class SceneManager {
public:
    // Ya no pedimos el shader del skybox
    SceneManager(Shader* main, LightManager* lm, Camera* cam);
    ~SceneManager();

    void loadHouse();
    void render(glm::mat4 view, glm::mat4 projection);

private:
    Shader* mainShader;
    LightManager* lightManager;
    Camera* camera;

    // Una sola lista para todo lo de la escena
    std::vector<Model*> sceneProps;

    void setupHouseLights();
};