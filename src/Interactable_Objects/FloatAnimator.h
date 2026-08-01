#pragma once

class FloatAnimator
{
public:
    explicit FloatAnimator(float speed = 90.0f);

    void setTarget(float target);
    void setCurrent(float value); // fija current y target al mismo valor, sin disparar animación

    void update(double deltaTime);

    float getCurrent() const;
    bool  isAnimating() const;

private:
    float current;
    float target;
    float speed;
};