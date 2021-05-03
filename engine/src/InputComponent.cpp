#include "Engine/ECS/Component/InputComponent.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/Engine.hpp"

#include <functional>

#include "Generated/InputComponent.rfk.h"
File_GENERATED

using namespace std;
using namespace GPE;

InputComponent::InputComponent(GameObject& owner) : Component(owner)
{
    m_key = Engine::getInstance()->inputManager.addComponent(*this);
}

InputComponent::InputComponent()
{
    m_key = Engine::getInstance()->inputManager.addComponent(*this);
}

InputComponent::InputComponent(InputComponent&& other) : Component(other.getOwner())
{
    m_functionMap = std::move(other.m_functionMap);
    m_key         = std::move(other.m_key);
    keyModeMap    = std::move(other.keyModeMap);

    Engine::getInstance()->inputManager.updateComponent(this, m_key);
}

InputComponent& InputComponent::operator=(InputComponent&& other)
{
    m_functionMap = std::move(other.m_functionMap);
    m_key         = std::move(other.m_key);
    keyModeMap    = std::move(other.keyModeMap);

    return static_cast<InputComponent&>(Component::operator=(std::move(other)));
}

InputComponent::~InputComponent()
{
    Engine::getInstance()->inputManager.removeComponent(m_key);
}

void InputComponent::fireAction(const std::string& action) noexcept
{
    auto it = m_functionMap.find(action);
    if (it != m_functionMap.end())
    {
        GPE::Function myfunc = it->second;
        myfunc();
    }
}

void InputComponent::setActive(bool newState) noexcept
{
    m_isActivated = newState;
    if (m_isActivated)
        m_key = Engine::getInstance()->inputManager.addComponent(*this);
    else
        Engine::getInstance()->inputManager.removeComponent(m_key);
}
