#include "SceneManager.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "../Interactable_Objects/HouseInteractableIds.h" 
#include "../Interactable_Objects/Interactable.h" 
#include "../Interactable_Objects/Door.h"

SceneManager::SceneManager(Shader* main, LightManager* lm, Camera* cam) {
    mainShader = main;
    lightManager = lm;
    camera = cam;

    loadHouse();
}

SceneManager::~SceneManager() {
    for (Model* prop : houseStaticProps) delete prop;
    for (auto& pair : houseDoorModels) delete pair.second;
}

void SceneManager::loadHouse() {
    std::cout << "[SceneManager] Cargando la casa y el vecindario..." << std::endl;

    //1. GENERAR EL VECINDARIO(4 Casas estratégicas para 60 FPS)
        vecindarioOffsets.clear();

    // Casa 1: La original (Donde spawnea el jugador)
    vecindarioOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

    // Casa 2: Al frente (Avanzando en el eje X)
    vecindarioOffsets.push_back(glm::vec3(-40.0f, 0.0f, 0.0f));

    // Casa 3: A la izquierda (Moviéndonos en el eje Z)
    vecindarioOffsets.push_back(glm::vec3(0.0f, 0.0f, -40.0f));

    // Casa 4: Frente a la de la izquierda (Diagonal)
    vecindarioOffsets.push_back(glm::vec3(-40.0f, 0.0f, -40.0f));

    // 2. CONSTRUIR EL SÚPER-PISO EN C++
    float fX0 = 0.2705f, fX1 = 0.4711f;
    float fY0 = 0.6036f, fY1 = 0.7586f;

    // Este es tu bloque base de 36 vértices (con el fix CCW de la cara superior)
    float baseFloor[] = {
        -0.5f, -0.5f, -0.5f,  fX0, fY0,   0.5f, -0.5f, -0.5f,  fX1, fY0,   0.5f,  0.5f, -0.5f,  fX1, fY1,
         0.5f,  0.5f, -0.5f,  fX1, fY1,  -0.5f,  0.5f, -0.5f,  fX0, fY1,  -0.5f, -0.5f, -0.5f,  fX0, fY0,
        -0.5f, -0.5f,  0.5f,  fX0, fY0,   0.5f, -0.5f,  0.5f,  fX1, fY0,   0.5f,  0.5f,  0.5f,  fX1, fY1,
         0.5f,  0.5f,  0.5f,  fX1, fY1,  -0.5f,  0.5f,  0.5f,  fX0, fY1,  -0.5f, -0.5f,  0.5f,  fX0, fY0,
        -0.5f,  0.5f,  0.5f,  fX0, fY1,  -0.5f,  0.5f, -0.5f,  fX1, fY1,  -0.5f, -0.5f, -0.5f,  fX1, fY0,
        -0.5f, -0.5f, -0.5f,  fX1, fY0,  -0.5f, -0.5f,  0.5f,  fX0, fY0,  -0.5f,  0.5f,  0.5f,  fX0, fY1,
         0.5f,  0.5f,  0.5f,  fX0, fY1,   0.5f,  0.5f, -0.5f,  fX1, fY1,   0.5f, -0.5f, -0.5f,  fX1, fY0,
         0.5f, -0.5f, -0.5f,  fX1, fY0,   0.5f, -0.5f,  0.5f,  fX0, fY0,   0.5f,  0.5f,  0.5f,  fX0, fY1,
        -0.5f, -0.5f, -0.5f,  fX0, fY0,   0.5f, -0.5f, -0.5f,  fX1, fY0,   0.5f, -0.5f,  0.5f,  fX1, fY1,
         0.5f, -0.5f,  0.5f,  fX1, fY1,  -0.5f, -0.5f,  0.5f,  fX0, fY1,  -0.5f, -0.5f, -0.5f,  fX0, fY0,
         // CARA SUPERIOR CCW
         -0.5f,  0.5f,  0.5f,  fX0, fY1,   0.5f,  0.5f,  0.5f,  fX1, fY1,   0.5f,  0.5f, -0.5f,  fX1, fY0,
          0.5f,  0.5f, -0.5f,  fX1, fY0,  -0.5f,  0.5f, -0.5f,  fX0, fY0,  -0.5f,  0.5f,  0.5f,  fX0, fY1
    };

    std::vector<float> giantFloor;

    // Armamos el piso dinámicamente en RAM (25,600 bloques convertidos en 1 matriz gigante)
    for (int x = -80; x <= 80; x++) {
        for (int z = -80; z <= 80; z++) {
            float offsetX = 260.0f + x;
            float offsetZ = -5.0f + z;

            for (int v = 0; v < 36; v++) {
                giantFloor.push_back(baseFloor[v * 5 + 0] + offsetX); // X modificado
                giantFloor.push_back(baseFloor[v * 5 + 1] - 1.0f);    // Y bajado a -1.0
                giantFloor.push_back(baseFloor[v * 5 + 2] + offsetZ); // Z modificado
                giantFloor.push_back(baseFloor[v * 5 + 3]);           // Textura U
                giantFloor.push_back(baseFloor[v * 5 + 4]);           // Textura V
            }
        }
    }

    totalFloorVertices = giantFloor.size() / 5; // Guardamos cuántos vértices son para el render()

    // 3. ENVIAR EL SÚPER-PISO A OPENGL (Se hace 1 sola vez)
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);

    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    // Usamos giantFloor.data() y calculamos el peso en bytes
    glBufferData(GL_ARRAY_BUFFER, giantFloor.size() * sizeof(float), giantFloor.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    stbi_set_flip_vertically_on_load(true);
    floorTexture = TextureFromFile("texture_floor.png", "Assets/textures", true);
    stbi_set_flip_vertically_on_load(false);

    for (Model* prop : houseStaticProps) delete prop;
    houseStaticProps.clear();

    for (auto& pair : houseDoorModels) delete pair.second;
    houseDoorModels.clear();

 

    // Modelos estáticos
    houseStaticProps.push_back(new Model("Assets/models/Casa/casa.obj"));
    // 1. Cargamos el modelo y lo atrapamos en nuestra variable
    bedModel = new Model("Assets/models/Bed/bed.obj");
    houseStaticProps.push_back(bedModel);
    houseStaticProps.push_back(new Model("Assets/models/Cupboard/cupboard.obj"));
    houseStaticProps.push_back(new Model("Assets/models/Desk/desk.obj"));
    houseStaticProps.push_back(new Model("Assets/models/Refrigerator/refrigerator.obj"));
    houseStaticProps.push_back(new Model("Assets/models/Shower/shower.obj"));
    houseStaticProps.push_back(new Model("Assets/models/Table/table.obj"));
    houseStaticProps.push_back(new Model("Assets/models/Toiled/toiled.obj"));
    houseStaticProps.push_back(new Model("Assets/models/Washbasin/washbasin.obj"));

    // Puertas animadas
    houseDoorModels[HouseInteractableIds::Door_CV1_P1] = new Model("Assets/models/P_CV1_p1/pcv1_p1.obj");
    houseDoorModels[HouseInteractableIds::Door_CV2_P1] = new Model("Assets/models/P_CV2_p1/pcv2_p1.obj");
    houseDoorModels[HouseInteractableIds::Door_CV3_P1] = new Model("Assets/models/P_CV3_p1/pcv3_p1.obj");
    houseDoorModels[HouseInteractableIds::Door_CV4_P1] = new Model("Assets/models/P_CV4_p1/pcv4_p1.obj");
    houseDoorModels[HouseInteractableIds::Door_CV5_P1] = new Model("Assets/models/P_CV5_p1/pcv5_p1.obj");
    houseDoorModels[HouseInteractableIds::Door_CV6_P1] = new Model("Assets/models/P_CV6_p1/pcv6_p1.obj");
    houseDoorModels[HouseInteractableIds::Door_CV7_P1] = new Model("Assets/models/P_CV7_p1/pcv7_p1.obj");
    houseDoorModels[HouseInteractableIds::Door_CV8_P1] = new Model("Assets/models/P_CV8_p1/pcv8_p1.obj");
    houseDoorModels[HouseInteractableIds::Door_CV9_P1] = new Model("Assets/models/P_CV9_p1/pcv9_p1.obj");

    houseDoorModels[HouseInteractableIds::Door_CV1_P2] = new Model("Assets/models/P_CV1_p2/pcv1_p2.obj");
    houseDoorModels[HouseInteractableIds::Door_CV2_P2] = new Model("Assets/models/P_CV2_p2/pcv2_p2.obj");
    houseDoorModels[HouseInteractableIds::Door_CV3_P2] = new Model("Assets/models/P_CV3_p2/pcv3_p2.obj");
    houseDoorModels[HouseInteractableIds::Door_CV4_P2] = new Model("Assets/models/P_CV4_p2/pcv4_p2.obj");

    houseDoorModels[HouseInteractableIds::Door_Bathroom] = new Model("Assets/models/P_Bathroom/p_bathroom.obj");
    houseDoorModels[HouseInteractableIds::Door_Bedroom] = new Model("Assets/models/P_Bedroom/p_bedroom.obj");
    houseDoorModels[HouseInteractableIds::Door_Cocina] = new Model("Assets/models/P_Cocina/p_cocina.obj");
    houseDoorModels[HouseInteractableIds::Door_Garage] = new Model("Assets/models/P_Garage/p_garage.obj");
    houseDoorModels[HouseInteractableIds::Door_Principal] = new Model("Assets/models/P_Principal/p_principal.obj");

    setupHouseLights();
}

void SceneManager::render(glm::mat4 view, glm::mat4 projection, const std::vector<Interactable*>& interactables) {
    mainShader->use();
    mainShader->setMat4("projection", projection);
    mainShader->setMat4("view", view);
    mainShader->setVec3("viewPos", camera->Position);

    lightManager->sendLightsToShader(*mainShader);
    mainShader->setVec3("flashLight.position", camera->Position);
    mainShader->setVec3("flashLight.direction", camera->Front);

    // ============================================================
    // PASADA 1: RENDEREAR SOLO LO OPACO
    // ============================================================
    mainShader->setBool("isTransparentPass", false);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    // --- 1. DIBUJAR SÚPER PISO (1 Draw Call) ---
    glVertexAttrib3f(1, 0.0f, 1.0f, 0.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    mainShader->setInt("texture_diffuse1", 0);

    glBindVertexArray(floorVAO);
    mainShader->setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, totalFloorVertices);
    glBindVertexArray(0);

    // --- 2. DIBUJAR VECINDARIO (OPTIMIZACIÓN DE CAMBIO DE ESTADO) ---
    // Invertimos los bucles: Iteramos por modelo, y luego lo pintamos en todas las posiciones
    for (Model* prop : houseStaticProps) {
        for (glm::vec3 offset : vecindarioOffsets) {
            glm::mat4 houseMatrix = glm::translate(glm::mat4(1.0f), offset);
            mainShader->setMat4("model", houseMatrix);
            prop->Draw(*mainShader);
        }
    }

    // Puertas
    for (glm::vec3 offset : vecindarioOffsets) {
        renderDoors(interactables, offset);
    }

    // ============================================================
    // PASADA 2: RENDEREAR SOLO LO TRANSPARENTE
    // ============================================================
    mainShader->setBool("isTransparentPass", true);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    // --- OPTIMIZACIÓN EXTREMA: SOLO ESTRUCTURA ---
    // Como solo la "casa.obj" tiene ventanas, evitamos dibujar camas y roperos en la pasada transparente.
    // Sabemos que la casa principal es el índice 0 en tu arreglo.
    if (!houseStaticProps.empty()) {
        Model* casaPrincipal = houseStaticProps[0];
        for (glm::vec3 offset : vecindarioOffsets) {
            glm::mat4 houseMatrix = glm::translate(glm::mat4(1.0f), offset);
            mainShader->setMat4("model", houseMatrix);
            casaPrincipal->Draw(*mainShader);
        }
    }

    // Las puertas sí tienen vidrio, así que deben ir en esta pasada
    for (glm::vec3 offset : vecindarioOffsets) {
        renderDoors(interactables, offset);
    }

    // Restaurar los estados por defecto
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

// Actualiza tu función renderDoors para recibir y aplicar el offset:
void SceneManager::renderDoors(const std::vector<Interactable*>& interactables, glm::vec3 houseOffset) {
    for (Interactable* obj : interactables) {
        if (!obj || obj->getType() != InteractableType::Door) continue;

        auto it = houseDoorModels.find(obj->getId());
        if (it == houseDoorModels.end()) continue;

        Door* door = static_cast<Door*>(obj);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, houseOffset); // 1. Mueve a la casa correcta
        modelMatrix = glm::translate(modelMatrix, door->getPosition()); // 2. Mueve a la posición de la puerta
        modelMatrix = glm::rotate(modelMatrix, glm::radians(door->getRotationY()), glm::vec3(0.0f, 1.0f, 0.0f));

        mainShader->setMat4("model", modelMatrix);
        it->second->Draw(*mainShader);
    }
}

void SceneManager::setupHouseLights() {
    PointLight* focoBath = new PointLight();
    focoBath->properties.diffuse = glm::vec3(1.0f, 0.9f, 0.8f);
    focoBath->setPosition(glm::vec3(262.692f, 6.29839f, -5.6183f));
    lightManager->addPointLight(focoBath);

    PointLight* focoBed = new PointLight();
    focoBed->properties.diffuse = glm::vec3(1.0f, 0.9f, 0.8f);
    focoBed->setPosition(glm::vec3(263.338f, 6.37f, 1.56341f));
    lightManager->addPointLight(focoBed);

    PointLight* focoCocina1 = new PointLight();
    focoCocina1->setPosition(glm::vec3(270.563f, 2.84878f, -5.3348f));
    lightManager->addPointLight(focoCocina1);

    PointLight* focoCocina2 = new PointLight();
    focoCocina2->setPosition(glm::vec3(270.187f, 6.29f, -0.825975f));
    lightManager->addPointLight(focoCocina2);

    PointLight* focoCV1P1 = new PointLight();
    focoCV1P1->setPosition(glm::vec3(264.525f, 2.8753f, 1.73852f));
    lightManager->addPointLight(focoCV1P1);

    PointLight* focoCV2P1 = new PointLight();
    focoCV2P1->setPosition(glm::vec3(261.209f, 2.59878f, -3.69f));
    lightManager->addPointLight(focoCV2P1);

    PointLight* focoCV3P1 = new PointLight();
    focoCV3P1->setPosition(glm::vec3(263.78f, 2.81878f, -5.60954f));
    lightManager->addPointLight(focoCV3P1);

    PointLight* focoCV4P1 = new PointLight();
    focoCV4P1->setPosition(glm::vec3(266.462f, 2.86878f, -4.77572f));
    lightManager->addPointLight(focoCV4P1);

    PointLight* focoCV5P1 = new PointLight();
    focoCV5P1->setPosition(glm::vec3(266.482f, 2.86878f, -7.26744f));
    lightManager->addPointLight(focoCV5P1);

    PointLight* focoCV6P1 = new PointLight();
    focoCV6P1->setPosition(glm::vec3(261.15f, 2.59878f, -6.32682f));
    lightManager->addPointLight(focoCV6P1);

    PointLight* focoCV8P1 = new PointLight();
    focoCV8P1->setPosition(glm::vec3(254.419f, 2.7353f, -4.53488f));
    lightManager->addPointLight(focoCV8P1);

    PointLight* focoCV9P1 = new PointLight();
    focoCV9P1->setPosition(glm::vec3(257.855f, 2.7353f, -4.60542f));
    lightManager->addPointLight(focoCV9P1);

    PointLight* focoGarage = new PointLight();
    focoGarage->setPosition(glm::vec3(256.109f, 2.7353f, 0.15f));
    lightManager->addPointLight(focoGarage);

    PointLight* focoCV1P2 = new PointLight();
    focoCV1P2->setPosition(glm::vec3(261.319f, 6.35819f, -1.89796f));
    lightManager->addPointLight(focoCV1P2);

    PointLight* focoCV2P2 = new PointLight();
    focoCV2P2->setPosition(glm::vec3(270.03f, 6.35f, -6.18294f));
    lightManager->addPointLight(focoCV2P2);

    PointLight* focoCV3P3 = new PointLight();
    focoCV3P3->setPosition(glm::vec3(266.578f, 6.35819f, -7.61872f));
    lightManager->addPointLight(focoCV3P3);

    PointLight* focoCV4P2 = new PointLight();
    focoCV4P2->setPosition(glm::vec3(266.564f, 6.33819f, -4.96717f));
    lightManager->addPointLight(focoCV4P2);
}