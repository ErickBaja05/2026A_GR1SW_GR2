#include "InteractableManager.h"
#include "HouseInteractableIds.h"

#include "Interactable.h"
#include "Door.h"
#include "LightSwitch.h"
#include "../Input/CollisionManager.h"

std::vector<std::unique_ptr<Interactable>> InteractableManager::createHouseInteractables(CollisionManager& collisionManager)
{
    std::vector<std::unique_ptr<Interactable>> interactables;

    // ===================== PUERTAS - PISO 1 =====================
    createDoorWithTrigger(HouseInteractableIds::Door_CV1_P1, glm::vec3(266.134f, 2.322f, -0.122242f), interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV2_P1, glm::vec3(262.271f, 2.272f, -2.2791f),   interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV3_P1, glm::vec3(263.903f, 2.272f, -3.73133f),  interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV4_P1, glm::vec3(267.053f, 2.272f, -3.73133f),  interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV5_P1, glm::vec3(267.321f, 2.272f, -8.20362f),  interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV6_P1, glm::vec3(260.671f, 2.272f, -5.24491f),  interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV7_P1, glm::vec3(259.907f, 2.272f, -2.2207f),   interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV8_P1, glm::vec3(255.103f, 2.222f, -3.44634f),  interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV9_P1, glm::vec3(256.409f, 2.222f, -3.40633f),  interactables, collisionManager);

    // ===================== PUERTAS - PISO 2 =====================
    createDoorWithTrigger(HouseInteractableIds::Door_CV1_P2, glm::vec3(263.151f, 5.672f, -2.18505f), interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV2_P2, glm::vec3(268.427f, 5.672f, -3.75903f), interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV3_P2, glm::vec3(267.871f, 5.672f, -8.55333f), interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_CV4_P2, glm::vec3(267.831f, 5.672f, -4.50971f), interactables, collisionManager);

    // ================ PUERTAS CON NOMBRE PROPIO ==================
    createDoorWithTrigger(HouseInteractableIds::Door_Bathroom,  glm::vec3(264.263f, 5.672f, -3.73293f),  interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_Bedroom,   glm::vec3(264.093f, 5.672f, -0.110922f), interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_Cocina,    glm::vec3(273.681f, 2.562f, -3.98411f),  interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_Garage,    glm::vec3(258.966f, 2.487f, 3.99366f),   interactables, collisionManager);
    createDoorWithTrigger(HouseInteractableIds::Door_Principal, glm::vec3(261.978f, 2.2f,   2.86866f),   interactables, collisionManager);

    // ===================== NUEVO: FOCOS DE LUZ =====================
    createLightCubeWithTrigger(HouseInteractableIds::Light_Bath, glm::vec3(262.692f, 6.29839f, -5.6183f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_Bed, glm::vec3(263.338f, 6.37f, 1.56341f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_Cocina1, glm::vec3(270.563f, 2.84878f, -5.3348f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_Cocina2, glm::vec3(270.187f, 6.29f, -0.825975f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV1_P1, glm::vec3(264.525f, 2.8753f, 1.73852f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV2_P1, glm::vec3(261.209f, 2.59878f, -3.69f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV3_P1, glm::vec3(263.78f, 2.81878f, -5.60954f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV4_P1, glm::vec3(266.462f, 2.86878f, -4.77572f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV5_P1, glm::vec3(266.482f, 2.86878f, -7.26744f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV6_P1, glm::vec3(261.15f, 2.59878f, -6.32682f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV8_P1, glm::vec3(254.419f, 2.7353f, -4.53488f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV9_P1, glm::vec3(257.855f, 2.7353f, -4.60542f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_Garage, glm::vec3(256.109f, 2.7353f, 0.15f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV1_P2, glm::vec3(261.319f, 6.35819f, -1.89796f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV2_P2, glm::vec3(270.03f, 6.35f, -6.18294f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV3_P3, glm::vec3(266.578f, 6.35819f, -7.61872f), interactables, collisionManager);
    createLightCubeWithTrigger(HouseInteractableIds::Light_CV4_P2, glm::vec3(266.564f, 6.33819f, -4.96717f), interactables, collisionManager);

    return interactables;
}

void InteractableManager::createDoorWithTrigger(int id, const glm::vec3& hingePosition,
    std::vector<std::unique_ptr<Interactable>>& outInteractables,
    CollisionManager& collisionManager)
{
    outInteractables.push_back(std::make_unique<Door>(id, hingePosition));
    float triggerHalfExtentXZ = 1.5f;
    float triggerHeightMin = -2.5f;
    float triggerHeightMax = 1.0f;

    Trigger trigger;
    trigger.name = "door_trigger_" + std::to_string(id);
    trigger.type = TriggerType::Door;
    trigger.box = BoundingBox{
        hingePosition + glm::vec3(-triggerHalfExtentXZ, triggerHeightMin, -triggerHalfExtentXZ),
        hingePosition + glm::vec3(triggerHalfExtentXZ, triggerHeightMax, triggerHalfExtentXZ)
    };
    trigger.enabled = true;
    trigger.targetId = id;

    collisionManager.addTrigger(trigger);
}

void InteractableManager::createLightCubeWithTrigger(int id, const glm::vec3& position,
    std::vector<std::unique_ptr<Interactable>>& outInteractables,
    CollisionManager& collisionManager)
{
    // Registramos la entidad lógica 'LightSwitch' para este ID
    outInteractables.push_back(std::make_unique<LightSwitch>(id, position));

    // Definimos un radio de interacción cómodo alrededor de la luz
    float triggerHalfExtent = 1.0f;

    Trigger trigger;
    trigger.name = "light_trigger_" + std::to_string(id);
    trigger.type = TriggerType::LightSwitch; // Para que GameLogic detecte que es un switch
    trigger.box = BoundingBox{
        position - glm::vec3(triggerHalfExtent), // Min corner
        position + glm::vec3(triggerHalfExtent)  // Max corner
    };
    trigger.enabled = true;
    trigger.targetId = id;

    collisionManager.addTrigger(trigger);
}


