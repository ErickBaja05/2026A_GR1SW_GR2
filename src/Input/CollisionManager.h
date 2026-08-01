#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

// Representa una caja de colisión
struct BoundingBox
{
    glm::vec3 min;
    glm::vec3 max;
};

// Zonas de interacción
enum class TriggerType
{
    Door,
    LightSwitch,
    TextureChanger
    // Agrega nuevos tipos aquí en el futuro
};

// Definición de una zona de interacción invisible (Trigger).
struct Trigger
{
    std::string name;
    TriggerType type;
    BoundingBox box;
    bool enabled;
    int targetId = -1; // para poder identificar a objetos similares (por ejemplo, las luces o puertas)
};

// A diferencia de Trigger, no dispara interacción ni tiene targetId: solo bloquea.
struct StaticCollider
{
    BoundingBox box;
};

class CollisionManager
{
public:
    CollisionManager() = default;
    ~CollisionManager() = default;

    // Agrega un nuevo trigger a la colección interna.
    void addTrigger(const Trigger& trigger);

    
    // Elimina todos los triggers almacenados.
    void clearTriggers();

    /**
     * Comprueba si una posición específica está dentro del BoundingBox
     * de algún trigger activo, y de haber varios candidatos, elige el que
     * mejor alineado esté con la dirección hacia la que mira el jugador.
     * * @param playerPosition Posición en el espacio 3D (usualmente la cámara).
     * @param playerForward Vector de dirección hacia el que mira el jugador (normalizado o no).
     * @return Trigger* Puntero al trigger activo más relevante encontrado en esa posición.
     * Retorna nullptr si no hay colisión con ningún trigger activo.
     */
    Trigger* checkCollision(const glm::vec3& playerPosition, const glm::vec3& playerForward);

    void addStaticCollider(const StaticCollider& collider);
    void clearStaticColliders();

    /**
     * Dada la posición actual y la posición deseada (antes de aplicarla),
     * devuelve la posición final permitida: si desiredPos choca con algún
     * StaticCollider, se recorta eje por eje (X y Z por separado) para
     * permitir deslizarse contra la pared en vez de detenerse en seco.
     * El eje Y no se toca por ahora (no hay gravedad/salto todavía).
     */
    glm::vec3 resolveMovement(const glm::vec3& currentPos, const glm::vec3& desiredPos) const;

private:
    std::vector<Trigger> m_triggers;
    std::vector<StaticCollider> m_staticColliders;

    /**
     * Función auxiliar matemática para comprobar si un punto 3D está
     * contenido dentro de un BoundingBox (AABB).
     * * @param point El punto a evaluar.
     * @param box La caja de colisión.
     * @return true si el punto está dentro, false en caso contrario.
     */
    bool isPointInsideBox(const glm::vec3& point, const BoundingBox& box) const;

    /**
     * Calcula qué tan alineado está el trigger con la dirección hacia la que
     * mira el jugador (producto punto normalizado, rango [-1, 1]).
     * Un valor más alto significa que el jugador está mirando más directamente hacia el trigger.
     */
    float getFacingScore(const glm::vec3& playerPosition, const glm::vec3& playerForward, const Trigger& trigger) const;
    bool collidesWithAnyStatic(const glm::vec3& point) const;
};