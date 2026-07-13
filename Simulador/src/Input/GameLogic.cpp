#include "GameLogic.h"

#include "CollisionManager.h"
#include "InputManager.h"
#include "../Engine/Window.h"
#include "../Engine/SceneManager.h"
#include "../Lighting/LightManager.h" 
#include "../Scene/camera.h"

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


void GameLogic::update()
{
    // Verifica que los punteros esenciales no sean nulos
    if (!camera || !collisionManager || !inputManager)
    {
        return;
    }

    // Obtener la posición actual de la cámara (jugador)
    glm::vec3 playerPosition = camera->getCameraPos();

    // Consultar al CollisionManager si el jugador está pisando algún Trigger activo
    const Trigger* activeTrigger = collisionManager->checkCollision(playerPosition);
    // Si no hay colisión con ningún trigger, terminamos el flujo de este frame
    //if (activeTrigger == nullptr)
    //{
    //    return;
    //}

    // 5. Si hay un trigger activo, verificamos si el jugador presionó la tecla de interacción

    bool isPressed = inputManager->isKeyPressed(interactionKey);
    if (isPressed && !wasInteractionKeyPressed)
    {
        // Si se presionó la tecla, procesamos la lógica de interacción de forma segmentada
        //processInteraction(activeTrigger);
        std::cout << "Tecla E presionada (Una sola vez)\n";
        if (sceneManager) {
            sceneManager->toggleScene(); // Usamos la nueva función maestra
        }
    }
    wasInteractionKeyPressed = isPressed;
}

void GameLogic::processInteraction(const Trigger* trigger)
{
    if (!trigger) return;

    // Enrutador
    switch (trigger->type)
    {
    case TriggerType::Door:
        handleDoor(trigger);
        break;

    case TriggerType::LightSwitch:
        handleLightSwitch(trigger);
        break;

    case TriggerType::TextureChanger:
        handleTextureChanger(trigger);
        break;

    default:
        break;
    }
}

void GameLogic::handleDoor(const Trigger* trigger)
{
    if (sceneManager)
    {
        // El SceneManager se encarga de la carga real del mapa e infraestructura interna
        sceneManager->loadHouse();
    }
}

void GameLogic::handleLightSwitch(const Trigger* trigger)
{
    if (lightManager)
    {
        // DISEÑO DE INTERFAZ FUTURA:
        // Pasando el puntero completo 'trigger', la función queda totalmente preparada para cuando
        // decidas extender la estructura 'Trigger' en tu proyecto.
        // Por ejemplo, en el futuro podrás agregarle campos como `trigger->lightIndex` o `trigger->targetState`.

        int lightIndex = 0;   // TODO: Reemplazar por trigger->lightIndex en el futuro
        bool targetState = true; // TODO: Reemplazar por la lógica de conmutación o estado del trigger

        lightManager->togglePointLight(lightIndex, targetState);
    }
}

void GameLogic::handleTextureChanger(const Trigger* trigger)
{
    // TODO: Implementar en fases posteriores de desarrollo.
    // Aquí coordinarás al SceneManager o TextureManager utilizando los datos del trigger correspondiente.
}