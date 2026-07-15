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

    return interactables;
}

void InteractableManager::createDoorWithTrigger(int id, const glm::vec3& hingePosition,
    std::vector<std::unique_ptr<Interactable>>& outInteractables,
    CollisionManager& collisionManager)
{
    outInteractables.push_back(std::make_unique<Door>(id, hingePosition));

    Trigger trigger;
    trigger.name = "door_trigger_" + std::to_string(id);
    trigger.type = TriggerType::Door;
    trigger.box = BoundingBox{
        hingePosition - glm::vec3(DOOR_TRIGGER_HALF_EXTENT_XZ, 0.0f, DOOR_TRIGGER_HALF_EXTENT_XZ),
        hingePosition + glm::vec3(DOOR_TRIGGER_HALF_EXTENT_XZ, DOOR_TRIGGER_HEIGHT, DOOR_TRIGGER_HALF_EXTENT_XZ)
    };
    trigger.enabled = true;
    trigger.targetId = id;

    collisionManager.addTrigger(trigger);
}

void InteractableManager::createLightSwitchWithTrigger(int id, const glm::vec3& position,
    std::vector<std::unique_ptr<Interactable>>& outInteractables,
    CollisionManager& collisionManager)
{
    outInteractables.push_back(std::make_unique<LightSwitch>(id, position));

    Trigger trigger;
    trigger.name = "lightswitch_trigger_" + std::to_string(id);
    trigger.type = TriggerType::LightSwitch;
    trigger.box = BoundingBox{
        position - glm::vec3(DOOR_TRIGGER_HALF_EXTENT_XZ, 0.0f, DOOR_TRIGGER_HALF_EXTENT_XZ),
        position + glm::vec3(DOOR_TRIGGER_HALF_EXTENT_XZ, DOOR_TRIGGER_HEIGHT, DOOR_TRIGGER_HALF_EXTENT_XZ)
    };
    trigger.enabled = true;
    trigger.targetId = id;

    collisionManager.addTrigger(trigger);
}
