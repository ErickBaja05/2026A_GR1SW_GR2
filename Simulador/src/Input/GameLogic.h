#pragma once

// Forward declarations para evitar inclusiones circulares y acelerar la compilación
class CollisionManager;
class InputManager;
class SceneManager;
class LightManager;
class Camera;
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
     */
    void update();

private:
    // Punteros a las dependencias externas
    CollisionManager* collisionManager;
    InputManager* inputManager;
    SceneManager* sceneManager;
    LightManager* lightManager;
    Camera* camera;

    // Código de la tecla configurada para interactuar (por ejemplo, GLFW_KEY_E)
    int interactionKey;

    /**
     * Enruta el trigger detectado hacia su respectivo manejador según su tipo.
     * @param trigger Puntero constante al trigger con el que se está interactuando.
     */
    void processInteraction(const Trigger* trigger);

    /**
     * Gestiona la interacción con triggers de tipo Door (Puertas).
     * @param trigger Puntero constante al trigger de la puerta.
     */
    void handleDoor(const Trigger* trigger);

    /**
     * Gestiona la interacción con triggers de tipo LightSwitch (Interruptores).
     * @param trigger Puntero constante al trigger del interruptor.
     */
    void handleLightSwitch(const Trigger* trigger);

    /**
     * Gestiona la interacción con triggers de tipo TextureChanger (Cambio de textura).
     * @param trigger Puntero constante al trigger del cambiador de textura.
     */
    void handleTextureChanger(const Trigger* trigger);
};