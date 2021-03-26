inline GameObject::GameObject(Scene& scene, const CreateArg& arg)
    : m_name{arg.name}, m_transform{DataChunk<TransformComponent>::getInstance()->add(*this, arg.transformArg)},
      m_pComponents{}, pOwnerScene{&scene}, m_parent{arg.parent}
{
}

template <typename T>
inline void GameObject::updateComponentLink(const T* oldPtr, T* newPtr) noexcept
{
    GPE_ASSERT(oldPtr != nullptr, "oldPtr cannot be nullptr");
    GPE_ASSERT(newPtr != nullptr, "newPtr cannot be nullptr");

    for (Component* pComponent : m_pComponents)
    {
        if (unlikely(pComponent == oldPtr))
        {
            pComponent = newPtr;
            return;
        }
    };
}

template <typename T, typename... Args>
inline T& GameObject::addComponent(Args&&... args) noexcept
{
    T& newComponent = DataChunk<T>::getInstance()->add(*this, std::forward<Args>(args)...);
    m_pComponents.emplace_back(&newComponent);
    return newComponent;
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

    for (std::list<std::unique_ptr<GameObject>>::iterator it = m_parent->children.begin();
         it != m_parent->children.end(); it++)
    {
        if (it->get() == this)
        {
            Log::getInstance()->log(stringFormat("Move %s from %s to %s", m_name.c_str(), m_parent->getName().c_str(),
                                                 newParent.getName().c_str()));

            newParent.children.emplace_back(std::move(*it)); // move the
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
    return m_transform;
}

inline constexpr TransformComponent& GameObject::getTransform() noexcept
{
    return m_transform;
}

template <typename T, typename... Args>
inline GameObject& GameObject::addChild(Args&&... args) noexcept
{
    // TODO : replace by data chunk
    std::unique_ptr<GameObject>& pChild = this->children.emplace_back(std::make_unique<T>(*pOwnerScene, args...));
    pChild->children                    = std::list<std::unique_ptr<GameObject>>();

    // pChild->update((*this).getModelMatrix());
    pChild->m_parent = this;
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

inline std::list<std::unique_ptr<GameObject>>::iterator GameObject::destroyChild(
    const std::list<std::unique_ptr<GameObject>>::iterator& it) noexcept
{
    return children.erase(it);
}

template <typename TUniqueComponentType>
inline void GameObject::destroyUniqueComponentNow() noexcept
{
    for (auto&& it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
    {
        TUniqueComponentType* checkedCompPtr = dynamic_cast<TUniqueComponentType*>(*it);

        if (unlikely(checkedCompPtr != nullptr))
        {
            DataChunk<TUniqueComponentType>::getInstance()->destroy(checkedCompPtr);
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
