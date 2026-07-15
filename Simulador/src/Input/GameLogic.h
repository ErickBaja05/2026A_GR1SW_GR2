#pragma once

#include <vector>

// Forward declarations para evitar inclusiones circulares y acelerar la compilación
class CollisionManager;
class InputManager;
class SceneManager;
class LightManager;
class Camera;
class Interactable;
struct Trigger;

/**
 * Clase encargada exclusivamente de coordinar las interacciones del juego.
 * Actúa como un puente intermedio entre el sistema de colisiones, la entrada del usuario
 * y los managers que modifican el estado de la escena y la iluminación.
 */
class GameLogic
{
public:
    GameLogic(CollisionManager* collisionManager, InputManager* inputManager, SceneManager* sceneManager,
        LightManager* lightManager, Camera* camera);

    ~GameLogic() = default;

    /**
     * Método principal de actualización de la lógica. Debe ser llamado en cada frame.
     * Realiza el flujo de control: posición -> colisión -> entrada -> interacción.
     * @param deltaTime Tiempo transcurrido desde el frame anterior, requerido para
     * que los Interactable (ej. Door) puedan animarse correctamente.
     */
    void update(double deltaTime);

    /**
     * Registra un objeto interactivo para que GameLogic pueda encontrarlo por id
     * cuando un Trigger lo referencie, y para que reciba update() cada frame.
     */
    void registerInteractable(Interactable* interactable);

    /**
     * Expone los interactables registrados para que SceneManager pueda consultar
     * su posición/rotación al renderizar (ej. puertas). GameLogic sigue siendo
     * el dueño de esta lista (no-owning para quien la lee).
     */
    const std::vector<Interactable*>& getInteractables() const { return interactables; }

private:
    // Punteros a las dependencias externas
    CollisionManager* collisionManager;
    InputManager* inputManager;
    SceneManager* sceneManager;
    LightManager* lightManager;
    Camera* camera;

    // Agrega esta variable en la sección 'private' de GameLogic.h
    bool wasInteractionKeyPressed = false;

    // Código de la tecla configurada para interactuar (por ejemplo, GLFW_KEY_E)
    int interactionKey;

    // Objetos interactivos registrados (Door, LightSwitch, ...). No-owning.
    std::vector<Interactable*> interactables;

    /**
     * Busca un Interactable registrado cuyo id coincida con targetId.
     * @return puntero al Interactable encontrado, o nullptr si no existe.
     */
    Interactable* findInteractable(int targetId) const;
};