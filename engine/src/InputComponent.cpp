#include "Engine/Core/Input/InputComponent.hpp"
#include "Engine/Core/Input/InputManagerGLFW.hpp"

#include <functional>

using namespace std;
using namespace GPE;

InputComponent::InputComponent(GameObject& owner) : Component(owner)
{
    m_key = InputManager::getInstance()->addComponent(this);
}

InputComponent::InputComponent(const InputComponent& other) noexcept : Component(other.m_gameObject)
{
}

InputComponent::InputComponent(InputComponent&& other) noexcept : Component(other.m_gameObject)
{
}

void InputComponent::fireAction(const std::string& action) noexcept
{
    auto it = m_functionMap.find(action);
    if (it != m_functionMap.end())
    {
        std::function<void()> myfunc = it->second;
        myfunc();
    }
}
