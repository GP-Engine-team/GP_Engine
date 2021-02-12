#include "Engine/Intermediate/ComponentChunk.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Core/Debug/Assert.hpp"

template <typename T>
void Engine::Intermediate::GameObject::updateComponentLink(const T* oldPtr, T* newPtr)
{
    GPE_ASSERT(oldPtr != nullptr, "oldPtr canno't be nullptr");
    GPE_ASSERT(newPtr != nullptr, "newPtr canno't be nullptr");

    for (Component* pComponent : m_pComponents)
    {
        T* checkedCompPtr = dynamic_cast<T*>(pComponent);

        if (checkedCompPtr == oldPtr)
            [[unlikely]]
        {
            pComponent = newPtr;
            return;
        }
    };
}

template <typename T, typename... Args>
T& Engine::Intermediate::GameObject::addComponent(Args&&... args) noexcept
{
    T& newComponent = ComponentChunk<T>::getInstance()->addComponent(*this, args...);
    m_pComponents.emplace_back(&newComponent);
    return newComponent;
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

inline const std::string& Engine::Intermediate::GameObject::getName() const noexcept
{
    return m_name;
}

inline void Engine::Intermediate::GameObject::setName(const char* newName) noexcept
{
    m_name = newName;
}

constexpr inline const Engine::Intermediate::TransformComponent& Engine::Intermediate::GameObject::getTransform()
    const noexcept
{
    return *m_pTransform;
}

constexpr inline Engine::Intermediate::TransformComponent& Engine::Intermediate::GameObject::getTransform() noexcept
{
    return *m_pTransform;
}

template <typename T, typename... Args>
Engine::Intermediate::GameObject& Engine::Intermediate::GameObject::addChild(Args&&... args) noexcept
{
    (*this).children.emplace_back(std::make_unique<T>(args...));
    (*this).children.back()->children = std::list<std::unique_ptr<GameObject>>();
    //(*this).children.back()->update((*this).getModelMatrix());
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
            toReturn.emplace_back(comp);
        }
    }
    return toReturn;
}

constexpr inline std::list<Engine::Intermediate::Component*>& Engine::Intermediate::GameObject::getComponents() noexcept
{
    return m_pComponents;
}

constexpr inline const std::list<Engine::Intermediate::Component*>& Engine::Intermediate::GameObject::getComponents()
    const noexcept
{
    return m_pComponents;
}

inline void Engine::Intermediate::GameObject::setTag(const std::string& newTag)
{
    m_tag = newTag;
}

constexpr inline const std::string& Engine::Intermediate::GameObject::getTag() const
{
    return m_tag;
}

inline std::list<std::unique_ptr<Engine::Intermediate::GameObject>>::iterator Engine::Intermediate::
    GameObject::destroyChild(const std::list<std::unique_ptr<GameObject>>::iterator& it) noexcept
{
    return children.erase(it);
}

template <typename TUniqueComponentType>
void Engine::Intermediate::GameObject::destroyImmediateUniqueComponent() noexcept
{
    for (auto&& it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
    {
        TUniqueComponentType* checkedCompPtr = dynamic_cast<TUniqueComponentType*>(*it);

        if (checkedCompPtr != nullptr)
            [[unlikely]]
        {
            ComponentChunk<TUniqueComponentType>::getInstance()->destroyComponent(checkedCompPtr);
            m_pComponents.erase(it);
            return;
        }
    };
}

inline void Engine::Intermediate::GameObject::setActive(bool newState)
{
    for (auto&& i : m_pComponents)
    {
        i->setActive(newState);
    }
}

inline std::list<Engine::Intermediate::Component*>::iterator Engine::Intermediate::GameObject::
    destroyComponent(const std::list<Component*>::iterator& it) noexcept
{
    return m_pComponents.erase(it);
}

inline void Engine::Intermediate::GameObject::destroy() noexcept
{
    /*set flag to be delete by it parent*/
    m_isDead = true;
    // m_isDirty = true;
}

inline void Engine::Intermediate::GameObject::destroyImmediate() noexcept
{
    parent->destroyChild(this);
}

constexpr inline bool Engine::Intermediate::GameObject::operator==(GameObject const& other) noexcept
{
    return (this == &other);
}

inline bool Engine::Intermediate::GameObject::compareTag(const std::string& toCompare) const noexcept
{
    if (toCompare.compare(m_tag) == 0)
        return true;
    return false;
}