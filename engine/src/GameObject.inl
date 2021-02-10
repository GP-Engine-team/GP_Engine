#include "Engine/Intermediate/ComponentChunk.hpp"
#include "Engine/Intermediate/GameObject.hpp"

template <typename T, typename... Args>
T& Engine::Intermediate::GameObject::addComponent(Args&&... args) noexcept
{
    m_pComponents.emplace_back(ComponentChunk<T>::addComponent(*this, args...));
    return *dynamic_cast<T*>(m_pComponents.back().get());
}

template <typename T>
T* Engine::Intermediate::GameObject::getComponent() noexcept
{
    for (std::unique_ptr<Component>& uniquePtrComponent : m_pComponents)
    {
        T* comp = dynamic_cast<T*>(uniquePtrComponent.get());

        if (comp != nullptr)
        {
            return comp;
        }
    }
    return nullptr;
}

template <typename T, typename... Args>
Engine::Intermediate::GameObject& Engine::Intermediate::GameObject::addChild(Args&&... args) noexcept
{
    (*this).children.emplace_back(std::make_unique<T>(args...));
    (*this).children.back()->children = std::list<std::unique_ptr<GameObject>>();
    (*this).children.back()->update((*this).getModelMatrix());
    (*this).children.back()->parent = this;
    return *(*this).children.back();
}

template <typename T>
std::vector<T*> Engine::Intermediate::GameObject::getComponents()
{
    std::vector<T*> toReturn;
    for (std::unique_ptr<Component>& uniquePtrComponent : m_pComponents)
    {
        T* comp = dynamic_cast<T*>(uniquePtrComponent.get());

        if (comp != nullptr)
        {
            toReturn.push_back(comp);
        }
    }
    return toReturn;
}

std::list<Engine::Intermediate::Component*>& Engine::Intermediate::GameObject::getComponents() noexcept
{
    return m_pComponents;
}
const std::list<Engine::Intermediate::Component*>& Engine::Intermediate::GameObject::getComponents()
    const noexcept
{
    return m_pComponents;
}

void Engine::Intermediate::GameObject::setTag(const std::string& newTag)
{
    m_tag = newTag;
}
std::string& Engine::Intermediate::GameObject::getTag()
{
    return m_tag;
}
