namespace GPE
{

inline GameObject::GameObject(Scene& scene, const CreateArg& arg)
    : m_name{arg.name}, m_pTransform{new TransformComponent(*this, arg.transformArg)}, m_pComponents{},
      pOwnerScene{&scene}, m_parent{arg.parent}, m_id{++m_currentID}
{
}

template <typename T, typename... Args>
inline T& GameObject::addComponent(Args&&... args) noexcept
{
    T* newComponent = new T(*this, std::forward<Args>(args)...);
    m_pComponents.emplace_back(newComponent);
    return *newComponent;
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

inline void GameObject::setParent(GameObject& newParent) noexcept
{
    GPE_ASSERT(m_parent, "You cannot move gameObject without parent");
    GPE_ASSERT(newParent.getParent() != this,
               "You cannot associate new parent if it's the chilf of the current entity (leak)");

    for (std::list<GameObject*>::iterator it = m_parent->children.begin(); it != m_parent->children.end(); it++)
    {
        if (*it == this)
        {
            Log::getInstance()->log(stringFormat("Move %s from %s to %s", m_name.c_str(), m_parent->getName().c_str(),
                                                 newParent.getName().c_str()));

            newParent.children.emplace_back(std::move(*it));
            m_parent->children.erase(it);
            break;
        }
    }

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
    pChild->getTransform().setDirty();
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
    for (auto&& i : m_pComponents)
    {
        i->setActive(newState);
    }
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