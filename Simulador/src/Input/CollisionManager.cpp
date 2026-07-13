#include "CollisionManager.h"

void CollisionManager::addTrigger(const Trigger& trigger)
{
    m_triggers.push_back(trigger);
}

void CollisionManager::clearTriggers()
{
    m_triggers.clear();
}

Trigger* CollisionManager::checkCollision(const glm::vec3& playerPosition)
{
    // Iteramos por referencia para poder devolver la dirección de memoria original
    for (auto& trigger : m_triggers)
    {
        // Solo evaluamos colisiones si el trigger está activo
        if (trigger.enabled)
        {
            if (isPointInsideBox(playerPosition, trigger.box))
            {
                return &trigger;
            }
        }
    }

    // Si terminamos de iterar y no hubo colisión, retornamos un puntero nulo
    return nullptr;
}

bool CollisionManager::isPointInsideBox(const glm::vec3& point, const BoundingBox& box) const
{
    // Comprobación AABB (Point vs AABB)
    // El punto debe estar entre el min y el max en los tres ejes simultáneamente
    bool isInsideX = (point.x >= box.min.x) && (point.x <= box.max.x);
    bool isInsideY = (point.y >= box.min.y) && (point.y <= box.max.y);
    bool isInsideZ = (point.z >= box.min.z) && (point.z <= box.max.z);

    return isInsideX && isInsideY && isInsideZ;
}