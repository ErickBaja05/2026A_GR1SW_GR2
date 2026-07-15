#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib> // Para generar números aleatorios (rand)
#include <ctime>   // Para la semilla (time)
#include "../Graphics/shader.h"
#include "../Scene/model.h" // ¡Corregido el path para evitar el error E1696!
#include "../Scene/camera.h"
#include "../Lighting/LightManager.h"
#include <stb_image.h>
#include <fstream>
#include <sstream>

// Estructura para guardar las instrucciones de cada clon
struct PropInstance {
    std::string name;
    glm::vec3 pos;
    float rotY;
    glm::vec3 scale;
};

// Forward declaration
class Interactable;

class SceneManager {
public:
    SceneManager(Shader* main, LightManager* lm, Camera* cam);
    ~SceneManager();

    void loadHouse();
    void render(glm::mat4 view, glm::mat4 projection, const std::vector<Interactable*>& interactables);

    std::vector<Model*> houseStaticProps;
    std::unordered_map<int, Model*> houseDoorModels;

private:
    Shader* mainShader;
    LightManager* lightManager;
    Camera* camera;
    Model* bedModel; // Guardamos el puntero para clonarlo después
    unsigned int floorVAO, floorVBO;
    unsigned int floorTexture;
    // Lista de offsets (desplazamientos) para el vecindario
    std::vector<glm::vec3> vecindarioOffsets;
    // Guardaremos la cantidad total de vértices del súper-piso
    int totalFloorVertices;

    // Catálogo de modelos base (para no cargar el mismo .obj dos veces)
    std::unordered_map<std::string, Model*> modelCatalog;

    // Aquí guardamos el centro original de cada modelo para "deshornearlo"
    std::unordered_map<std::string, glm::vec3> originalPositions;

    // La lista de todos los muebles extraídos del .txt
    std::vector<PropInstance> sceneLayout;

    // Función para leer el txt
    void loadLayoutData(const std::string& filepath);

    void setupHouseLights();
    void renderDoors(const std::vector<Interactable*>& interactables, glm::vec3 houseOffset);
};