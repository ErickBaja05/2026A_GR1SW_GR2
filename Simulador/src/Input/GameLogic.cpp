#include "GameLogic.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "../Engine/SceneManager.h"
#include "../Lighting/LightManager.h" 
#include "../Scene/camera.h"
#include "../Interactable_Objects/Interactable.h"
#include "../Interactable_Objects/LightSwitch.h"
#include <iostream>

#ifndef GLFW_KEY_E
#define GLFW_KEY_E 69
#endif

#ifndef GLFW_KEY_6
#define GLFW_KEY_6 54
#endif
#ifndef GLFW_KEY_7
#define GLFW_KEY_7 55
#endif

GameLogic::GameLogic(CollisionManager* collisionManager, InputManager* inputManager, SceneManager* sceneManager, LightManager* lightManager, Camera* camera) {
    this->collisionManager = collisionManager;
    this->inputManager = inputManager;
    this->sceneManager = sceneManager;
    this->lightManager = lightManager;
    this->camera = camera;
    this->interactionKey = GLFW_KEY_E;
}

void GameLogic::update(double deltaTime) {
    if (!camera || !collisionManager || !inputManager) return;

    if (inputManager->isKeyJustPressed(GLFW_KEY_6) && sceneManager) {
        sceneManager->ciclarTexturaInterior();
    }
    if (inputManager->isKeyJustPressed(GLFW_KEY_7) && sceneManager) {
        sceneManager->ciclarTexturaExterior();
    }

    // 1. Obtener la posición y vector frontal usando los atributos públicos de Camera
    glm::vec3 playerPosition = camera->getCameraPos();
    glm::vec3 playerForward = camera->getCameraFront();

    // 2. Consultar Trigger activo
    const Trigger* activeTrigger = collisionManager->checkCollision(playerPosition, playerForward);

    // 3. Llamar a isKeyJustPressed SIEMPRE para que el InputManager se actualice
    bool ePressed = inputManager->isKeyJustPressed(interactionKey);
    
    // 4. Evaluar interacción
    if (activeTrigger && ePressed) {
        std::cout << "Interactuando en zona ID: " << activeTrigger->targetId << "\n";

        if (activeTrigger->type == TriggerType::LightSwitch && lightManager) {
            std::cout << "Hola\n";
            lightManager->togglePointLight(activeTrigger->targetId);
        }
        else {
            // Para otros objetos físicos e interactuables (como las puertas)
            Interactable* target = findInteractable(activeTrigger->targetId);
            if (target && target->isEnabled()) {
                target->interact();
            }
        }
    }

    // 5. Actualizar animaciones de TODOS los objetos (ej. Puertas abriéndose)
    for (Interactable* obj : interactables) {
        obj->update(deltaTime);
    }
}

void GameLogic::registerInteractable(Interactable* interactable) {
    if (interactable) {
        interactables.push_back(interactable);
    }
}

Interactable* GameLogic::findInteractable(int targetId) const {
    for (Interactable* obj : interactables) {
        if (obj->getId() == targetId) {
            return obj;
        }
    }
    return nullptr;
}