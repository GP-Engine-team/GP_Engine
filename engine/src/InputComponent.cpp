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
    updateToSystem();
}

InputComponent::InputComponent()
{

}

InputComponent::~InputComponent()
{
    setActive(false);
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

void InputComponent::updateToSystem() noexcept
{
    if (m_isActivated)
        m_key = Engine::getInstance()->inputManager.addComponent(*this);
    else
        Engine::getInstance()->inputManager.removeComponent(m_key);
}
