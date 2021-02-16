#include "Engine/Intermediate/GameObject.hpp"

template <typename T>
void GameObject::updateComponentLink(const T* oldPtr, T* newPtr)
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
T& GameObject::addComponent(Args&&... args) noexcept
{
    T& newComponent = DataChunk<T>::getInstance()->addComponent(*this, std::forward<Args>(args)...);
    m_pComponents.emplace_back(&newComponent);
    return newComponent;
}

template <typename T>
T* GameObject::getComponent() noexcept
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

inline const std::string& GameObject::getName() const noexcept
{
    return m_name;
}

inline void GameObject::setName(const char* newName) noexcept
{
    m_name = newName;
}

constexpr inline const TransformComponent& GameObject::getTransform()
    const noexcept
{
    return *m_pTransform;
}

constexpr inline TransformComponent& GameObject::getTransform() noexcept
{
    return *m_pTransform;
}

template <typename T, typename... Args>
GameObject& GameObject::addChild(Args&&... args) noexcept
{
    (*this).children.emplace_back(std::make_unique<T>(args...));
    (*this).children.back()->children = std::list<std::unique_ptr<GameObject>>();
    //(*this).children.back()->update((*this).getModelMatrix());
    (*this).children.back()->parent = this;
    return *(*this).children.back();
}

template <typename T>
std::vector<T*> GameObject::getComponents()
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

constexpr inline std::list<Component*>& GameObject::getComponents() noexcept
{
    return m_pComponents;
}

constexpr inline const std::list<Component*>& GameObject::getComponents()
    const noexcept
{
    return m_pComponents;
}

inline void GameObject::setTag(const std::string& newTag)
{
    m_tag = newTag;
}

constexpr inline const std::string& GameObject::getTag() const
{
    return m_tag;
}

inline std::list<std::unique_ptr<GameObject>>::iterator 
    GameObject::destroyChild(const std::list<std::unique_ptr<GameObject>>::iterator& it) noexcept
{
    return children.erase(it);
}

template <typename TUniqueComponentType>
void GameObject::destroyImmediateUniqueComponent() noexcept
{
    for (auto&& it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
    {
        TUniqueComponentType* checkedCompPtr = dynamic_cast<TUniqueComponentType*>(*it);

        if (checkedCompPtr != nullptr)
            [[unlikely]]
        {
            DataChunk<TUniqueComponentType>::getInstance()->destroyComponent(checkedCompPtr);
            m_pComponents.erase(it);
            return;
        }
    };
}

inline void GameObject::setActive(bool newState)
{
    for (auto&& i : m_pComponents)
    {
        i->setActive(newState);
    }
}

inline std::list<Component*>::iterator GameObject::
    destroyComponent(const std::list<Component*>::iterator& it) noexcept
{
    return m_pComponents.erase(it);
}

inline void GameObject::destroy() noexcept
{
    /*set flag to be delete by it parent*/
    m_isDead = true;
    // m_isDirty = true;
}

inline void GameObject::destroyImmediate() noexcept
{
    parent->destroyChild(this);
}

constexpr inline bool GameObject::operator==(GameObject const& other) noexcept
{
    return (this == &other);
}

inline bool GameObject::compareTag(const std::string& toCompare) const noexcept
{
    if (toCompare.compare(m_tag) == 0)
        return true;
    return false;
}