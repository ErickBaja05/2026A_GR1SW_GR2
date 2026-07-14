#include "GameLogic.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "../Engine/SceneManager.h"
#include "../Lighting/LightManager.h" 
#include "../Scene/camera.h"
#include <iostream>

#ifndef GLFW_KEY_E
#define GLFW_KEY_E 69
#endif

GameLogic::GameLogic(CollisionManager* collisionManager, InputManager* inputManager, SceneManager* sceneManager, LightManager* lightManager, Camera* camera) {
    this->collisionManager = collisionManager;
    this->inputManager = inputManager;
    this->sceneManager = sceneManager;
    this->lightManager = lightManager;
    this->camera = camera;
    this->interactionKey = GLFW_KEY_E;
}

void GameLogic::update() {
    if (!camera || !collisionManager || !inputManager) return;

    glm::vec3 playerPosition = camera->getCameraPos();
    const Trigger* activeTrigger = collisionManager->checkCollision(playerPosition);

    bool isPressed = inputManager->isKeyPressed(interactionKey);
    if (isPressed && !wasInteractionKeyPressed) {
        // Solo reacciona si el jugador está dentro de una caja de colisión
        if (activeTrigger != nullptr) {
            std::cout << "Interactuando con: " << activeTrigger->name << "\n";
            processInteraction(activeTrigger);
        }
        else {
            std::cout << "No hay nada con qué interactuar aquí.\n";
        }
    }
    wasInteractionKeyPressed = isPressed;
}

void GameLogic::processInteraction(const Trigger* trigger) {
    if (!trigger) return;

    switch (trigger->type) {
    case TriggerType::LightSwitch:
        handleLightSwitch(trigger);
        break;
    case TriggerType::TextureChanger:
        // TODO para los objetos a recolectar
        std::cout << "¡Objeto recolectado!\n";
        break;
    default:
        break;
    }
}

void GameLogic::handleDoor(const Trigger* trigger) {
    std::cout << "La puerta está bloqueada, tienes que encontrar la salida.\n";
}

void GameLogic::handleLightSwitch(const Trigger* trigger) {
    if (lightManager) {
        // En un futuro trigger->targetId te dirá qué foco apagar
        lightManager->togglePointLight(0, false);
    }
}

void GameLogic::handleTextureChanger(const Trigger* trigger) {
}