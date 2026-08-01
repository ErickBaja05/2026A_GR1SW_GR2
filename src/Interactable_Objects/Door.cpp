#include "Door.h"

Door::Door(int id, const glm::vec3& position, float closeAngleDeg, float openAngleDeg, const glm::vec3& rotationAxis)
    : Interactable(id, InteractableType::Door, position)
    , state(DoorState::Closed)
    , closeAngle(closeAngleDeg)
    , openAngle(openAngleDeg)
    , rotationAxis(rotationAxis)
    , angleAnimator()
{
    // El animator debe iniciar exactamente en closeAngle, sin animación pendiente
    angleAnimator.setTarget(closeAngleDeg);
}

void Door::interact()
{
    // interact() solo decide la transición de estado según el estado actual.
    // Mientras la puerta está en movimiento (Opening/Closing) se ignora la interacción.
    switch (state)
    {
    case DoorState::Closed:
        open();
        break;

    case DoorState::Open:
        close();
        break;

    case DoorState::Opening:
    case DoorState::Closing:
        // No se invierte la animación a mitad de recorrido
        break;
    }
}

void Door::update(double deltaTime)
{
    angleAnimator.update(deltaTime);

    // Solo hay transición de estado pendiente si estamos animando
    if (!angleAnimator.isAnimating())
    {
        if (state == DoorState::Opening)
        {
            state = DoorState::Open;
        }
        else if (state == DoorState::Closing)
        {
            state = DoorState::Closed;
        }
    }
}

DoorState Door::getState() const
{
    return state;
}

float Door::getRotationY() const
{
    return angleAnimator.getCurrent();
}

const glm::vec3& Door::getRotationAxis() const
{
    return rotationAxis;
}

void Door::open()
{
    state = DoorState::Opening;
    angleAnimator.setTarget(openAngle);
}

void Door::close()
{
    state = DoorState::Closing;
    angleAnimator.setTarget(closeAngle);
}