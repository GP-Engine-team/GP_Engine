#include "Engine/Core/Input/InputComponent.hpp"
#include "Engine/Core/Input/InputManagerGLFW.hpp"

using namespace std;
using namespace GPE;

InputComponent::InputComponent(GameObject& owner) : Component(owner)
{
    m_key = InputManager::GetInstance()->addComponent(this);
}

void InputComponent::bindAction(const string& action, void* function)
{
}

void InputComponent::fireAction(const std::string& action)
{
}