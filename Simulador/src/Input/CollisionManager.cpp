#include "CollisionManager.h"

void CollisionManager::addTrigger(const Trigger& trigger)
{
    m_triggers.push_back(trigger);
}

void CollisionManager::clearTriggers()
{
    m_triggers.clear();
}

Trigger* CollisionManager::checkCollision(const glm::vec3& playerPosition, const glm::vec3& playerForward)
{
    Trigger* bestTrigger = nullptr;
    float bestScore = -2.0f; // por debajo del mínimo posible de un producto punto normalizado (-1)

    for (auto& trigger : m_triggers)
    {
        // Solo evaluamos colisiones si el trigger está activo
        if (!trigger.enabled)
        {
            continue;
        }

        if (!isPointInsideBox(playerPosition, trigger.box))
        {
            continue;
        }

        float score = getFacingScore(playerPosition, playerForward, trigger);

        // Nos quedamos con el candidato mejor alineado con la mirada del jugador.
        // Si solo hay un candidato, este simplemente termina siendo el elegido.
        if (score > bestScore)
        {
            bestScore = score;
            bestTrigger = &trigger;
        }
    }

    return bestTrigger;
}

float CollisionManager::getFacingScore(const glm::vec3& playerPosition, const glm::vec3& playerForward, const Trigger& trigger) const
{
    glm::vec3 triggerCenter = (trigger.box.min + trigger.box.max) * 0.5f;
    glm::vec3 toTrigger = triggerCenter - playerPosition;

    float distance = glm::length(toTrigger);

    // Si el jugador está prácticamente sobre el centro del trigger, la dirección
    // no es significativa: no penalizamos por orientación en ese caso.
    if (distance < 0.0001f)
    {
        return 1.0f;
    }

    glm::vec3 forwardNormalized = glm::normalize(playerForward);
    glm::vec3 toTriggerNormalized = toTrigger / distance;

    return glm::dot(toTriggerNormalized, forwardNormalized);
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