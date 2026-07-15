#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Interactable;
class CollisionManager;

/**
 * Clase estática responsable de construir todos los objetos interactivos
 * de la casa (puertas, interruptores de luz, y futuros tipos) junto con
 * sus Trigger correspondientes en CollisionManager.
 *
 * No se instancia: solo agrupa la lógica de creación en un único lugar,
 * evitando repetir 17+ veces el patrón "crear objeto + crear trigger + id compartido".
 *
 * La memoria de los Interactable creados es devuelta por valor (vector de
 * unique_ptr); quien llama a createHouseInteractables() se vuelve el dueño
 * real de esa memoria (ej. Game/main.cpp), consistente con cómo el resto
 * del proyecto maneja sus managers (Window, LightManager, SceneManager, etc).
 */
class InteractableManager
{
public:
    InteractableManager() = delete;
    InteractableManager(const InteractableManager&) = delete;
    InteractableManager& operator=(const InteractableManager&) = delete;

    /**
     * Crea todos los interactables de la casa (por ahora, las 18 puertas)
     * y registra un Trigger por cada uno en collisionManager, usando el
     * mismo id definido en HouseInteractableIds.h.
     *
     * @param collisionManager Manager donde se registran los Trigger de cada objeto creado.
     * @return vector con la propiedad de todos los Interactable creados.
     */
    static std::vector<std::unique_ptr<Interactable>> createHouseInteractables(CollisionManager& collisionManager);

private:
    /**
     * Crea un Door en hingePosition, lo agrega a outInteractables, y registra
     * su Trigger correspondiente (mismo id) en collisionManager.
     */
    static void createDoorWithTrigger(int id, const glm::vec3& hingePosition,
        std::vector<std::unique_ptr<Interactable>>& outInteractables,
        CollisionManager& collisionManager);

    /**
     * Crea un LightSwitch en position, lo agrega a outInteractables, y registra
     * su Trigger correspondiente (mismo id) en collisionManager.
     * (Reservado para cuando se agreguen las coordenadas de los interruptores;
     * el patrón es idéntico al de createDoorWithTrigger).
     */
    static void createLightSwitchWithTrigger(int id, const glm::vec3& position,
        std::vector<std::unique_ptr<Interactable>>& outInteractables,
        CollisionManager& collisionManager);

    // Medio-extensión (en unidades de mundo) de la zona de trigger alrededor
    // de cada bisagra de puerta, en los ejes X/Z. Ajustar según la escala real
    // de la casa una vez probado en el motor.
    static constexpr float DOOR_TRIGGER_HALF_EXTENT_XZ = 1.5f;
    static constexpr float DOOR_TRIGGER_HEIGHT = 2.2f;
};
