#include "SceneManager.h"
#include <iostream>


SceneManager::SceneManager(Shader* main, LightManager* lm, Camera* cam) {
    mainShader = main;
    lightManager = lm;
    camera = cam;

    // Al instanciar, cargamos directamente la casa
    loadHouse();
}

SceneManager::~SceneManager() {
    for (Model* prop : sceneProps) delete prop;
    sceneProps.clear();
}

void SceneManager::loadHouse() {
    std::cout << "[SceneManager] Cargando Escena Principal (Casa y Objetos)..." << std::endl;

    for (Model* prop : sceneProps) delete prop;
    sceneProps.clear();

    // 1. La Casa y Cuartos
    sceneProps.push_back(new Model("Assets/models/Casa/casa.obj"));
    sceneProps.push_back(new Model("Assets/models/Bed/bed.obj"));
    sceneProps.push_back(new Model("Assets/models/Cupboard/cupboard.obj"));
    sceneProps.push_back(new Model("Assets/models/Desk/desk.obj"));
    sceneProps.push_back(new Model("Assets/models/P_Bathroom/p_bathroom.obj"));
    sceneProps.push_back(new Model("Assets/models/P_Bedroom/p_bedroom.obj"));
    sceneProps.push_back(new Model("Assets/models/P_Cocina/p_cocina.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV1_p1/pcv1_p1.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV1_p2/pcv1_p2.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV2_p1/pcv2_p1.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV2_p2/pcv2_p2.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV3_p1/pcv3_p1.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV3_p2/pcv3_p2.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV4_p1/pcv4_p1.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV4_p2/pcv4_p2.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV5_p1/pcv5_p1.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV6_p1/pcv6_p1.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV7_p1/pcv7_p1.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV8_p1/pcv8_p1.obj"));
    sceneProps.push_back(new Model("Assets/models/P_CV9_p1/pcv9_p1.obj"));
    sceneProps.push_back(new Model("Assets/models/P_Garage/p_garage.obj"));
    sceneProps.push_back(new Model("Assets/models/P_Principal/p_principal.obj"));
    sceneProps.push_back(new Model("Assets/models/Refrigerator/refrigerator.obj"));
    sceneProps.push_back(new Model("Assets/models/Shower/shower.obj"));
    sceneProps.push_back(new Model("Assets/models/Table/table.obj"));
    sceneProps.push_back(new Model("Assets/models/Toiled/toiled.obj"));
    sceneProps.push_back(new Model("Assets/models/Washbasin/washbasin.obj"));

    // 2. TUS 5 OBJETOS DE SKETCHFAB (Malditos / A recolectar)
    // Descomenta y pon las rutas reales cuando los tengan
    // sceneProps.push_back(new Model("Assets/models/SketchfabObj1/obj1.obj"));
    // sceneProps.push_back(new Model("Assets/models/SketchfabObj2/obj2.obj"));
    // sceneProps.push_back(new Model("Assets/models/SketchfabObj3/obj3.obj"));
    // sceneProps.push_back(new Model("Assets/models/SketchfabObj4/obj4.obj"));
    // sceneProps.push_back(new Model("Assets/models/SketchfabObj5/obj5.obj"));

    // 3. Luces
    setupHouseLights();

    // 4. Posición Inicial
    if (camera) camera->Position = glm::vec3(264.0f, 3.0f, -2.0f);
}

void SceneManager::render(glm::mat4 view, glm::mat4 projection) {
    mainShader->use();
    mainShader->setMat4("projection", projection);
    mainShader->setMat4("view", view);

    // Posición de cámara para que funcionen los brillos del Phong (Specular)
    mainShader->setVec3("viewPos", camera->Position);

    lightManager->sendLightsToShader(*mainShader);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    mainShader->setMat4("model", modelMatrix);

    // Activamos la transparencia (Blender/Vidrio)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (Model* prop : sceneProps) {
        prop->Draw(*mainShader);
    }

    glDisable(GL_BLEND);
}

void SceneManager::setupHouseLights() {
    // Exactamente el mismo código que me enviaste con los 15 PointLights.
    // Solo pégalo aquí adentro tal cual estaba.
    PointLight* focoBath = new PointLight();
    focoBath->properties.diffuse = glm::vec3(1.0f, 0.9f, 0.8f);
    focoBath->setPosition(glm::vec3(262.692f, 6.29839f, -5.6183f));
    lightManager->addPointLight(focoBath);

    // ... Pega el resto de los PointLights de tu código original ...
}


void SceneManager::setupHouseLights() {
    // Inyectamos las coordenadas exactas de Anderson ya procesadas para OpenGL

    // 1. Foco Bathroom[cite: 3]
    PointLight* focoBath = new PointLight();
    focoBath->properties.diffuse = glm::vec3(1.0f, 0.9f, 0.8f);
    focoBath->setPosition(glm::vec3(262.692f, 6.29839f, -5.6183f));
    lightManager->addPointLight(focoBath);

    // 2. Foco Bedroom[cite: 3]
    PointLight* focoBed = new PointLight();
    focoBed->properties.diffuse = glm::vec3(1.0f, 0.9f, 0.8f);
    focoBed->setPosition(glm::vec3(263.338f, 6.37f, 1.56341f));
    lightManager->addPointLight(focoBed);

    // 3. Focos Cocina[cite: 3]
    PointLight* focoCocina1 = new PointLight();
    focoCocina1->setPosition(glm::vec3(270.563f, 2.84878f, -5.3348f));
    lightManager->addPointLight(focoCocina1);

    PointLight* focoCocina2 = new PointLight();
    focoCocina2->setPosition(glm::vec3(270.187f, 6.29f, -0.825975f));
    lightManager->addPointLight(focoCocina2);

    // 4. Focos Cuartos Varios (CV)[cite: 3]
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
