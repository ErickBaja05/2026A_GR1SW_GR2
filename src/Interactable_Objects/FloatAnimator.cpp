#include "FloatAnimator.h"
#include <algorithm>
#include <cmath>

FloatAnimator::FloatAnimator(float speed)
    : current(0.0f)
    , target(0.0f)
    , speed(speed)
{
}

void FloatAnimator::setTarget(float newTarget)
{
    target = newTarget;
}

void FloatAnimator::setCurrent(float value)
{
    current = value;
    target = value;
}

void FloatAnimator::update(double deltaTime)
{
    if (!isAnimating())
    {
        return;
    }

    float direction = (target > current) ? 1.0f : -1.0f;
    float step = speed * static_cast<float>(deltaTime) * direction;

    // Si el paso se pasa del objetivo, se recorta exactamente al valor objetivo
    if (std::abs(step) >= std::abs(target - current))
    {
        current = target;
    }
    else
    {
        current += step;
    }
}

float FloatAnimator::getCurrent() const
{
    return current;
}

bool FloatAnimator::isAnimating() const
{
    return current != target;
}