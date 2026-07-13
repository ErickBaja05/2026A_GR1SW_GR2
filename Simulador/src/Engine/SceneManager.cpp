#include "SceneManager.h"
#include <iostream>
// Asegúrate de que stb_image se incluye aquí si la función loadCubemap lo usa
#include <stb_image.h> 
#include "../Graphics/LightManager.h"

SceneManager::SceneManager(Shader* main, Shader* sky, LightManager* lm) {
    mainShader = main;
    skyboxShader = sky;
    lightManager = lm;
    currentState = SceneState::NEIGHBORHOOD;

    setupSkybox(); // Inicializamos el cubo gigante de fondo
}

SceneManager::~SceneManager() {
    for (Model* prop : neighborhoodProps) delete prop;
    for (Model* prop : houseProps) delete prop;
}

void SceneManager::loadNeighborhood() {
    std::cout << "[SceneManager] Cargando Vecindario..." << std::endl;
    for (Model* prop : houseProps) delete prop;
    houseProps.clear();

    // Aquí agregas todos los .obj de afuera
    //neighborhoodProps.push_back(new Model("Assets/models/vecindario/ciudad.obj"));
    // neighborhoodProps.push_back(new Model("Assets/models/vecindario/arbol1.obj"));

    // Configuramos luz de día
    lightManager->setDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f));

    currentState = SceneState::NEIGHBORHOOD;
}

void SceneManager::loadHouse() {
    std::cout << "[SceneManager] Entrando a la casa..." << std::endl;
    for (Model* prop : neighborhoodProps) delete prop;
    neighborhoodProps.clear();

    // 1. Cargas la estructura principal (Paredes, piso, techo)
    houseProps.push_back(new Model("Assets/models/casa/casa.obj"));

    // 2. Cargas los objetos interactivos o separados que mandó Anderson
    //houseProps.push_back(new Model("Assets/models/refrigerator/refrigerator.obj"));
    //houseProps.push_back(new Model("Assets/models/bed/bed.obj"));
    //houseProps.push_back(new Model("Assets/models/kitchen/kitchen.obj"));

    setupHouseLights();

    currentState = SceneState::INSIDE_HOUSE;
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

void SceneManager::render(glm::mat4 view, glm::mat4 projection) {
    // 1. Dibujamos los objetos sólidos
    mainShader->use();
    mainShader->setMat4("projection", projection);
    mainShader->setMat4("view", view);

    // Jorge envía la información de las luces al shader de los objetos[cite: 6]
    lightManager->sendLightsToShader(*mainShader);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
    mainShader->setMat4("model", modelMatrix);

    if (currentState == SceneState::NEIGHBORHOOD) {
        for (Model* prop : neighborhoodProps) prop->Draw(*mainShader);
    }
    else if (currentState == SceneState::INSIDE_HOUSE) {
        for (Model* prop : houseProps) prop->Draw(*mainShader);
    }

    // 2. Dibujamos el Skybox AL FINAL para optimizar memoria
    glDepthFunc(GL_LEQUAL); // Cambiamos función de profundidad para que el fondo se dibuje detrás de todo
    skyboxShader->use();
    view = glm::mat4(glm::mat3(view)); // Le quitamos la traslación a la cámara para que el cielo no se mueva
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", projection);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // Restauramos configuración original
}

// === CÓDIGO TÉCNICO DE GENERACIÓN DE SKYBOX ===
void SceneManager::setupSkybox() {
    float skyboxVertices[] = {
        // Coordenadas de los 36 vértices de un cubo
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Las fotos de Anderson
    std::vector<std::string> faces = {
        "Assets/skybox/right.jpg", "Assets/skybox/left.jpg",
        "Assets/skybox/top.jpg", "Assets/skybox/bottom.jpg",
        "Assets/skybox/front.jpg", "Assets/skybox/back.jpg"
    };
    cubemapTexture = loadCubemap(faces);
}

unsigned int SceneManager::loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Fallo al cargar imagen de Skybox: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    // Parámetros obligatorios para Skybox
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}