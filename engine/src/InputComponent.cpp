#include "Engine/Core/Input/InputComponent.hpp"
#include "Engine/Core/Input/InputManagerGLFW.hpp"

using namespace std;
using namespace GPE;

InputComponent::InputComponent(GameObject& owner) : Component(owner)
{
    m_key = InputManager::GetInstance()->addComponent(this);
}

InputComponent::InputComponent(const InputComponent& other) noexcept : Component(other.m_gameObject)
{
    m_key = -1;
}

InputComponent::InputComponent(InputComponent&& other) noexcept : Component(other.m_gameObject)
{
    m_key = -1;
}

void InputComponent::bindAction(const string& action, const std::function<void()>& function)
{
    m_functionMap.emplace(action, function);
}

void InputComponent::fireAction(const std::string& action)
{
    auto it = m_functionMap.find(action);
    if (it != m_functionMap.end())
    {
        std::function<void()> myfunc = it->second;
        myfunc();
    }
}

void InputComponent::bindAxis(const std::string& action, const std::function<void(const GPM::Vec3& axis)>& function)
{
    //m_axisFunctionMap.emplace(action, function);
}