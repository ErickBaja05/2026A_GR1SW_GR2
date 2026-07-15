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
    std::cout << "[SceneManager] Cargando la casa..." << std::endl;

    for (Model* prop : houseStaticProps) delete prop;
    houseStaticProps.clear();

    for (auto& pair : houseDoorModels) delete pair.second;
    houseDoorModels.clear();

    // Modelos estáticos
    houseStaticProps.push_back(new Model("Assets/models/Casa/casa.obj"));
    houseStaticProps.push_back(new Model("Assets/models/Bed/bed.obj"));
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

    // Parches de la linterna y luz base
    mainShader->setVec3("flashLight.position", camera->Position);
    mainShader->setVec3("flashLight.direction", camera->Front);
    mainShader->setVec3("dirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render estático
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    mainShader->setMat4("model", identityMatrix);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (Model* prop : houseStaticProps) prop->Draw(*mainShader);

    // Render dinámico (Puertas animadas)
    renderDoors(interactables);

    glDisable(GL_BLEND);
}

void SceneManager::renderDoors(const std::vector<Interactable*>& interactables) {
    for (Interactable* obj : interactables) {
        if (!obj || obj->getType() != InteractableType::Door) {
            continue;
        }

        auto it = houseDoorModels.find(obj->getId());
        if (it == houseDoorModels.end()) continue;

        Door* door = static_cast<Door*>(obj);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, door->getPosition());
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