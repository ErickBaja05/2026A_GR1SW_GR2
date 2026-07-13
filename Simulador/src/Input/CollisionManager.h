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
     * de algún trigger activo.
     * * @param playerPosition Posición en el espacio 3D (usualmente la cámara).
     * @return Trigger* Puntero al primer trigger activo encontrado en esa posición.
     * Retorna nullptr si no hay colisión.
     */
    Trigger* checkCollision(const glm::vec3& playerPosition);

private:
    std::vector<Trigger> m_triggers;

    /**
     * Función auxiliar matemática para comprobar si un punto 3D está
     * contenido dentro de un BoundingBox (AABB).
     * * @param point El punto a evaluar.
     * @param box La caja de colisión.
     * @return true si el punto está dentro, false en caso contrario.
     */
    bool isPointInsideBox(const glm::vec3& point, const BoundingBox& box) const;
};