#include "GameLogic.h"

#include "CollisionManager.h"
#include "InputManager.h"
#include "../Engine/Window.h"
#include "../Engine/SceneManager.h"
#include "../Lighting/LightManager.h" 
#include "../Scene/camera.h"
#include "../Interactable_Objects/Interactable.h"
#include "../Interactable_Objects/LightSwitch.h"
#include <iostream>

// Definición local de la tecla 'E' para evitar depender directamente del header de GLFW en el entorno global
#ifndef GLFW_KEY_E
#define GLFW_KEY_E 69
#endif

GameLogic::GameLogic(CollisionManager* collisionManager, InputManager* inputManager, SceneManager* sceneManager,
    LightManager* lightManager, Camera* camera) {
    this->collisionManager = collisionManager;
    this->inputManager = inputManager;
    this->sceneManager = sceneManager;
    this->lightManager = lightManager;
    this->camera = camera;
    this->interactionKey = GLFW_KEY_E;
}


void GameLogic::update(double deltaTime)
{
    // Verifica que los punteros esenciales no sean nulos
    if (!camera || !collisionManager || !inputManager)
    {
        return;
    }

    // Obtener la posición actual de la cámara (jugador)
    glm::vec3 playerPosition = camera->getCameraPos();
    glm::vec3 playerForward = camera->getCameraFront();

    // Consultar al CollisionManager si el jugador está pisando algún Trigger activo
    const Trigger* activeTrigger = collisionManager->checkCollision(playerPosition, playerForward);
    // Si hay un trigger activo y se presionó la tecla de interacción,
    // buscamos el Interactable correspondiente por targetId y delegamos en él.
    if (activeTrigger) std::cout << "hola\n";
    if (activeTrigger && inputManager->isKeyJustPressed(interactionKey))
    {
        std::cout << "Mark\n";
        Interactable* target = findInteractable(activeTrigger->targetId);
        if (target && target->isEnabled())
        {
            std::cout << "Id: " << target->getId() << "\n";
            target->interact();

            // Si el objeto que interactuó es un interruptor de luz, GameLogic
            // (no LightSwitch) es quien consulta su nuevo estado y notifica
            // al sistema de iluminación.
            if (target->getType() == InteractableType::LightSwitch && lightManager)
            {
                LightSwitch* lightSwitch = static_cast<LightSwitch*>(target);

                // Se usa el id del propio LightSwitch como índice de la luz asociada.
                // Si en el futuro un interruptor debe controlar un índice distinto
                // de luz, conviene agregar un campo dedicado (ej. lightIndex) a LightSwitch
                // o mantener un mapeo id -> lightIndex en este mismo método.
                lightManager->togglePointLight(lightSwitch->getId(), lightSwitch->getIsOn());
            }
        }
    }

    // Actualiza TODOS los interactables registrados, no solo el que colisiona:
    // una puerta que empezó a abrirse debe seguir animando aunque el jugador
    // se aleje del trigger.
    for (Interactable* obj : interactables)
    {
        obj->update(deltaTime);
    }
}

void GameLogic::registerInteractable(Interactable* interactable)
{
    if (interactable)
    {
        interactables.push_back(interactable);
    }
}

Interactable* GameLogic::findInteractable(int targetId) const
{
    for (Interactable* obj : interactables)
    {
        if (obj->getId() == targetId)
        {
            return obj;
        }
    }
    return nullptr;
}