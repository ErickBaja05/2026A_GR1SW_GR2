//#include "GameLogic.h"
//
//#include "CollisionManager.h"
//#include "InputManager.h"
//#include "../Engine/Window.h"
//#include "../Engine/SceneManager.h"
//#include "../Lighting/LightManager.h" 
//#include "../Scene/camera.h"
//#include "../Interactable_Objects/Interactable.h"
//#include "../Interactable_Objects/LightSwitch.h"
//
//
//// Definición local de la tecla 'E' para evitar depender directamente del header de GLFW en el entorno global
//#ifndef GLFW_KEY_E
//#define GLFW_KEY_E 69
//#endif
//
//GameLogic::GameLogic(CollisionManager* collisionManager, InputManager* inputManager, SceneManager* sceneManager,
//    LightManager* lightManager, Camera* camera) {
//    this->collisionManager = collisionManager;
//    this->inputManager = inputManager;
//    this->sceneManager = sceneManager;
//    this->lightManager = lightManager;
//    this->camera = camera;
//    this->interactionKey = GLFW_KEY_E;
//}
//
//
//void GameLogic::update(double deltaTime)
//{
//    // Verifica que los punteros esenciales no sean nulos
//    if (!camera || !collisionManager || !inputManager)
//    {
//        return;
//    }
//
//    // Obtener la posición actual de la cámara (jugador)
//    glm::vec3 playerPosition = camera->getCameraPos();
//    glm::vec3 playerForward = camera->getCameraFront();
//
//    // Consultar al CollisionManager si el jugador está pisando algún Trigger activo
//    const Trigger* activeTrigger = collisionManager->checkCollision(playerPosition, playerForward);
//    // Si hay un trigger activo y se presionó la tecla de interacción,
//    // buscamos el Interactable correspondiente por targetId y delegamos en él.
//    if (activeTrigger && inputManager->isKeyPressed(interactionKey))
//    {
//        Interactable* target = findInteractable(activeTrigger->targetId);
//        if (target && target->isEnabled())
//        {
//            target->interact();
//
//            // Si el objeto que interactuó es un interruptor de luz, GameLogic
//            // (no LightSwitch) es quien consulta su nuevo estado y notifica
//            // al sistema de iluminación.
//            if (target->getType() == InteractableType::LightSwitch && lightManager)
//            {
//                LightSwitch* lightSwitch = static_cast<LightSwitch*>(target);
//
//                // Se usa el id del propio LightSwitch como índice de la luz asociada.
//                // Si en el futuro un interruptor debe controlar un índice distinto
//                // de luz, conviene agregar un campo dedicado (ej. lightIndex) a LightSwitch
//                // o mantener un mapeo id -> lightIndex en este mismo método.
//                lightManager->togglePointLight(lightSwitch->getId(), lightSwitch->getIsOn());
//            }
//        }
//    }
//
//    // Actualiza TODOS los interactables registrados, no solo el que colisiona:
//    // una puerta que empezó a abrirse debe seguir animando aunque el jugador
//    // se aleje del trigger.
//    for (Interactable* obj : interactables)
//    {
//        obj->update(deltaTime);
//    }
//}
//
//void GameLogic::registerInteractable(Interactable* interactable)
//{
//    if (interactable)
//    {
//        interactables.push_back(interactable);
//    }
//}
//
//Interactable* GameLogic::findInteractable(int targetId) const
//{
//    for (Interactable* obj : interactables)
//    {
//        if (obj->getId() == targetId)
//        {
//            return obj;
//        }
//    }
//    return nullptr;
//}

#include "GameLogic.h"

#include <iostream>          // <-- NUEVO: para los prints de debug
#include "CollisionManager.h"
#include "InputManager.h"
#include "../Engine/Window.h"
#include "../Engine/SceneManager.h"
#include "../Lighting/LightManager.h" 
#include "../Scene/camera.h"
#include "../Interactable_Objects/Interactable.h"
#include "../Interactable_Objects/LightSwitch.h"
#include "../Interactable_Objects/Door.h"   // <-- NUEVO: para imprimir el estado de la puerta

#ifndef GLFW_KEY_E
#define GLFW_KEY_E 69
#endif

// Helper temporal solo para debug en consola
static const char* doorStateToString(DoorState state)
{
    switch (state)
    {
    case DoorState::Closed:  return "Closed";
    case DoorState::Opening: return "Opening";
    case DoorState::Open:    return "Open";
    case DoorState::Closing: return "Closing";
    }
    return "Unknown";
}

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
    if (!camera || !collisionManager || !inputManager)
    {
        return;
    }

    glm::vec3 playerPosition = camera->getCameraPos();
    glm::vec3 playerForward = camera->getCameraFront();

    const Trigger* activeTrigger = collisionManager->checkCollision(playerPosition, playerForward);

    // === DEBUG TEMPORAL: reporta entradas/salidas de zona (solo en cambios, no cada frame) ===
    static const Trigger* lastTrigger = nullptr;
    if (activeTrigger != lastTrigger)
    {
        if (activeTrigger)
        {
            std::cout << "[DEBUG] Entraste a zona: \"" << activeTrigger->name
                << "\" (targetId=" << activeTrigger->targetId << ") - presiona E para interactuar\n";
        }
        else if (lastTrigger)
        {
            std::cout << "[DEBUG] Saliste de la zona: \"" << lastTrigger->name << "\"\n";
        }
        lastTrigger = activeTrigger;
    }
    // === FIN DEBUG TEMPORAL ===

    if (activeTrigger && inputManager->isKeyJustPressed(interactionKey))
    {
        Interactable* target = findInteractable(activeTrigger->targetId);
        if (target && target->isEnabled())
        {
            target->interact();

            // === DEBUG TEMPORAL: reporta el resultado de la interacción ===
            std::cout << "[DEBUG] Interactuaste con id=" << target->getId() << " -> ";
            if (target->getType() == InteractableType::Door)
            {
                Door* door = static_cast<Door*>(target);
                std::cout << "nuevo estado de puerta: " << doorStateToString(door->getState()) << "\n";
            }
            else if (target->getType() == InteractableType::LightSwitch)
            {
                LightSwitch* ls = static_cast<LightSwitch*>(target);
                std::cout << "luz ahora: " << (ls->getIsOn() ? "ENCENDIDA" : "APAGADA") << "\n";
            }
            // === FIN DEBUG TEMPORAL ===

            if (target->getType() == InteractableType::LightSwitch && lightManager)
            {
                LightSwitch* lightSwitch = static_cast<LightSwitch*>(target);
                lightManager->togglePointLight(lightSwitch->getId(), lightSwitch->getIsOn());
            }
        }
    }

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