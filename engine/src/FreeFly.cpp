#include "Engine/Resources/Script/FreeFly.hpp"

#include "Engine/Engine.hpp"

void GPE::FreeFly::update(float deltaTime) 
{
    m_speed = 1;

    if (Engine::getInstance()->inputManager.getCursor().deltaPos.sqrLength() > 0.1)
    {
        rotate(Engine::getInstance()->inputManager.getCursor().deltaPos);
    }
}