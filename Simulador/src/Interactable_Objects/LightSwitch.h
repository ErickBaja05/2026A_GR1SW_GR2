#pragma once
#include <glm/glm.hpp>
#include "Interactable.h"

class LightSwitch : public Interactable
{
public:
    LightSwitch(int id, const glm::vec3& position);

    void interact() override;
    void update(double deltaTime) override;

    bool getIsOn() const;

private:
    bool isOn;
};