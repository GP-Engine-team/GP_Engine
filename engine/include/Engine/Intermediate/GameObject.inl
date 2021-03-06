﻿namespace GPE
{
template <typename T, typename... Args>
inline T& GameObject::addComponent(Args&&... args) noexcept
{
    T* newComponent = new T(*this, std::forward<Args>(args)...);
    m_pComponents.emplace_back(newComponent);
    return *newComponent;
}

template <typename T>
T& GameObject::getOrCreateComponent()
{
    if (T* comp = getComponent<T>())
    {
        return *comp;
    }
    return addComponent<T>();
}

inline Component* GameObject::addExistingComponent(Component* pExistingComponent) noexcept
{
    return m_pComponents.emplace_back(pExistingComponent);
}

template <typename T>
inline T* GameObject::getComponent() noexcept
{
    for (auto&& pComponent : m_pComponents)
    {
        T* comp = dynamic_cast<T*>(pComponent);

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

inline const GameObject* GameObject::getParent() const noexcept
{
    return m_parent;
}

inline GameObject* GameObject::getParent() noexcept
{
    return m_parent;
}

inline bool GameObject::isDead() const
{
    return m_isDead;
}

inline void GameObject::forceSetParent(GameObject& newParent) noexcept
{
    m_parent = &newParent;
}

inline void GameObject::setName(const char* newName) noexcept
{
    m_name = newName;
}

inline constexpr const TransformComponent& GameObject::getTransform() const noexcept
{
    return *m_pTransform;
}

inline constexpr TransformComponent& GameObject::getTransform() noexcept
{
    return *m_pTransform;
}

template <typename... Args>
inline GameObject& GameObject::addChild(Args&&... args) noexcept
{
    GameObject* pChild = children.emplace_back(new GameObject(*pOwnerScene, args...));

    pChild->m_parent = this;
    pChild->getTransform().update(getTransform().getModelMatrix());
    return *pChild;
}

template <typename T>
inline std::vector<T*> GameObject::getComponents() noexcept
{
    std::vector<T*> toReturn;
    for (auto&& pComponent : m_pComponents)
    {
        T* comp = dynamic_cast<T*>(pComponent);

        if (comp != nullptr)
        {
            toReturn.emplace_back(comp);
        }
    }
    return toReturn;
}

inline constexpr std::list<Component*>& GameObject::getComponents() noexcept
{
    return m_pComponents;
}

inline constexpr const std::list<Component*>& GameObject::getComponents() const noexcept
{
    return m_pComponents;
}

inline void GameObject::setTag(const std::string& newTag) noexcept
{
    m_tag = newTag;
}

inline constexpr const std::string& GameObject::getTag() const noexcept
{
    return m_tag;
}

inline std::list<GameObject*>::iterator GameObject::destroyChild(const std::list<GameObject*>::iterator& it) noexcept
{
    delete *it;
    return children.erase(it);
}

template <typename TUniqueComponentType>
inline void GameObject::destroyUniqueComponentNow() noexcept
{
    const std::list<Component*>::const_iterator end = m_pComponents.end();

    for (std::list<Component*>::iterator it = m_pComponents.begin(); it != end; ++it)
    {
        TUniqueComponentType* checkedCompPtr = dynamic_cast<TUniqueComponentType*>(*it);

        if (unlikely(checkedCompPtr != nullptr))
        {
            delete checkedCompPtr;
            m_pComponents.erase(it);
            return;
        }
    };
}

inline void GameObject::setActive(bool newState)
{
    m_isActive = newState;
    for (auto&& i : m_pComponents)
    {
        i->setActive(newState);
    }

    for (auto&& child : children)
    {
        child->setActive(newState);
    }
}

inline bool GameObject::isActivated()
{
    return m_isActive;
}

inline std::list<Component*>::iterator GameObject::destroyComponent(const std::list<Component*>::iterator& it) noexcept
{
    delete *it;
    return m_pComponents.erase(it);
}

inline void GameObject::destroy() noexcept
{
    /*set flag to be delete by it parent*/
    m_isDead = true;
}

inline void GameObject::destroyNow() noexcept
{
    m_parent->destroyChild(this);
}

inline constexpr bool GameObject::operator==(GameObject const& other) noexcept
{
    return (this == &other);
}

inline bool GameObject::compareTag(const std::string& toCompare) const noexcept
{
    return (!toCompare.compare(m_tag));
}

unsigned int GameObject::getID() const noexcept
{
    return m_id;
}

} // namespace GPE