#pragma once
#include <glm/glm.hpp>

class SceneNode {
protected:
    glm::vec3 position;
    // Aquí a futuro puedes agregar rotation y scale

public:
    SceneNode() : position(glm::vec3(0.0f)) {}
    virtual ~SceneNode() = default;

    // Métodos que Jorge necesita para sus luces
    void setPosition(glm::vec3 pos) { position = pos; }
    glm::vec3 getGlobalPosition() const { return position; }
};
