#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "../Graphics/shader.h"
#include "../Scene/model.h" // ¡Corregido el path para evitar el error E1696!
#include "../Scene/camera.h"
#include "../Lighting/LightManager.h"

// Forward declaration
class Interactable;

class SceneManager {
public:
    // Modificar constructor para recibir el segundo shader
    SceneManager(Shader* main, Shader* lightCube, LightManager* lm, Camera* cam);
    // ...
    ~SceneManager();

    void loadHouse();
    void render(glm::mat4 view, glm::mat4 projection, const std::vector<Interactable*>& interactables);

    std::vector<Model*> houseStaticProps;
    std::unordered_map<int, Model*> houseDoorModels;
    void updateLights(float time);

private:
    Shader* mainShader;
    LightManager* lightManager;
    Shader* lightCubeShader; // NUEVO
    Camera* camera;
    Model* bedModel; // Guardamos el puntero para clonarlo después
    PointLight* focoCocina1;
    unsigned int lightCubeVAO; // NUEVO
    unsigned int lightCubeVBO; // NUEVO
    void setupLightCube();     // NUEVO
    void setupHouseLights();
    void renderDoors(const std::vector<Interactable*>& interactables);
};