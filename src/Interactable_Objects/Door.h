#pragma once
#include <glm/glm.hpp>
#include "Interactable.h"
#include "FloatAnimator.h"

// Door.h
enum class DoorState { Closed, Opening, Open, Closing };

class Door : public Interactable
{
public:
    Door(int id, const glm::vec3& position,
        float closeAngleDeg = 0.0f, float openAngleDeg = 90.0f,
        const glm::vec3& rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f));

    void interact() override;
    void update(double deltaTime) override;

    DoorState getState() const;
    float getRotationY() const; // ángulo actual del animator, para que SceneManager arme la transform
    const glm::vec3& getRotationAxis() const; // eje sobre el que rota (Y por defecto, X para el garage)

protected:
    void open();
    void close();

private:
    DoorState     state;
    float         closeAngle;
    float         openAngle;
    glm::vec3     rotationAxis;
    FloatAnimator angleAnimator;
};