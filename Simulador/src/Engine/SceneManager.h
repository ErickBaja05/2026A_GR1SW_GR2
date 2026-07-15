#pragma once
#include <vector>
#include <string>
#include "../Graphics/shader.h"
#include "../Scene/model.h"
#include "../Scene/camera.h"
#include "../Lighting/LightManager.h"
#include "../Scene/camera.h" // ¡Importante incluir la cámara!

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>

// Forward declaration: SceneManager solo necesita llamar getType()/getId()/getRotationY()
// sobre estos objetos durante render(), no los crea ni los posee.
class Interactable;

enum class SceneState {
    NEIGHBORHOOD,
    INSIDE_HOUSE
};

class SceneManager {
public:
    // Ya no pedimos el shader del skybox
    SceneManager(Shader* main, LightManager* lm, Camera* cam);
    ~SceneManager();

    std::vector<Model*> neighborhoodProps;
    std::vector<Model*> houseStaticProps; // props sin transformación individual (paredes, muebles, etc.)
    std::unordered_map<int, Model*> houseDoorModels; // indexado por el mismo id de HouseInteractableIds.h

private:
    Shader* mainShader;
    LightManager* lightManager;
    Camera* camera;

    // Una sola lista para todo lo de la escena
    std::vector<Model*> sceneProps;

    void setupHouseLights();
    unsigned int loadCubemap(std::vector<std::string> faces);
    void setupSkybox();

    /**
     * Dibuja cada puerta con su propia matriz de transformación (traslación a
     * su bisagra + rotación sobre Y según Door::getRotationY()), usando el
     * Model* correspondiente en houseDoorModels según Interactable::getId().
     */
    void renderDoors(const std::vector<Interactable*>& interactables);

public:
    // Ahora recibe la cámara
    SceneManager(Shader* main, Shader* sky, LightManager* lm, Camera* cam);
    ~SceneManager();

    void loadNeighborhood();
    void loadHouse();
    void toggleScene(); // Función que usará Josue

    /**
     * @param interactables Lista de objetos interactivos (Door, LightSwitch, ...) para que las
     * puertas se dibujen con su rotación actual. No se posee esta lista, solo se consulta.
     */
    void render(glm::mat4 view, glm::mat4 projection, const std::vector<Interactable*>& interactables);
    SceneState getCurrentState() { return currentState; }
};