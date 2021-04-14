#include "Engine/ECS/Component/InputComponent.hpp"

File_GENERATED

#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/Engine.hpp"

#include <functional>

using namespace std;
using namespace GPE;

InputComponent::InputComponent(GameObject& owner) : Component(owner)
{
    m_key = Engine::getInstance()->inputManager.addComponent(this);
}

InputComponent::InputComponent() 
{
    m_key = Engine::getInstance()->inputManager.addComponent(this);
}


InputComponent::InputComponent(InputComponent&& other) : Component(other.getOwner())
{
    m_functionMap = std::move(other.m_functionMap);
    m_key         = std::move(other.m_key);
    m_keyModeMap  = std::move(other.m_keyModeMap);

    Engine::getInstance()->inputManager.updateComponent(this, m_key);
}

InputComponent& InputComponent::operator=(InputComponent&& other)
{
    m_functionMap = std::move(other.m_functionMap);
    m_key         = std::move(other.m_key);
    m_keyModeMap  = std::move(other.m_keyModeMap);

    return static_cast<InputComponent&>(Component::operator=(std::move(other)));
}

InputComponent::~InputComponent()
{
    Engine::getInstance()->inputManager.removeComponent(m_key);
    DataChunk<InputComponent>::getInstance()->destroy(this);
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
