#include "Interactable.h"

Interactable::Interactable(int id, InteractableType type, const glm::vec3& position)
    : id(id)
    , type(type)
    , position(position)
    , enabled(true)
{
}

int Interactable::getId() const
{
    return id;
}

InteractableType Interactable::getType() const
{
    return type;
}

const glm::vec3& Interactable::getPosition() const
{
    return position;
}

bool Interactable::isEnabled() const
{
    return enabled;
}

void Interactable::setEnabled(bool value)
{
    enabled = value;
}