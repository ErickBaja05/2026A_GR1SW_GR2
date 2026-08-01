#pragma once
#include <glm/glm.hpp>

enum class InteractableType {
    Door,
    LightSwitch,
    // Window, Gate, Elevator, NPC, TextureChanger... a futuro
};

class Interactable
{
public:
    virtual ~Interactable() = default;

    virtual void interact() = 0;
    virtual void update(double deltaTime) = 0;

    int getId() const;
    InteractableType getType() const;
    const glm::vec3& getPosition() const;
    bool isEnabled() const;
    void setEnabled(bool value);

protected:
    Interactable(int id, InteractableType type, const glm::vec3& position);

    int              id;
    InteractableType type;
    glm::vec3        position;
    bool             enabled;
};