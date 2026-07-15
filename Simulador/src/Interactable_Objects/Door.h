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
        float closeAngleDeg = 0.0f, float openAngleDeg = 90.0f);

    void interact() override;
    void update(double deltaTime) override;

    DoorState getState() const;
    float getRotationY() const; // ángulo actual del animator, para que SceneManager arme la transform

protected:
    void open();
    void close();

private:
    DoorState     state;
    float         closeAngle;
    float         openAngle;
    FloatAnimator angleAnimator;
};