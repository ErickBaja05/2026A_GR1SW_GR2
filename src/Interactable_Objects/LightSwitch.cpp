#include "LightSwitch.h"

LightSwitch::LightSwitch(int id, const glm::vec3& position)
    : Interactable(id, InteractableType::LightSwitch, position)
    , isOn(false)
{
}

void LightSwitch::interact()
{
    // Único comportamiento: alternar el propio estado.
    // GameLogic es quien luego consulta getIsOn() y notifica a LightManager.
    isOn = !isOn;
}

void LightSwitch::update(double deltaTime)
{
    // Sin animación ni lógica dependiente del tiempo por ahora.
    // Se mantiene el método (requerido por Interactable) para futuras
    // extensiones, por ejemplo una pequeña animación visual del interruptor.
}

bool LightSwitch::getIsOn() const
{
    return isOn;
}