#pragma once
#include <vector>

class CollisionManager;
class InputManager;
class SceneManager;
class LightManager;
class Camera;
class Interactable;
struct Trigger;

class GameLogic {
public:
    GameLogic(CollisionManager* collisionManager, InputManager* inputManager, SceneManager* sceneManager, LightManager* lightManager, Camera* camera);
    ~GameLogic() = default;

    void update(double deltaTime);
    void registerInteractable(Interactable* interactable);
    const std::vector<Interactable*>& getInteractables() const { return interactables; }

private:
    CollisionManager* collisionManager;
    InputManager* inputManager;
    SceneManager* sceneManager;
    LightManager* lightManager;
    Camera* camera;

    int interactionKey;
    std::vector<Interactable*> interactables;

    Interactable* findInteractable(int targetId) const;
};