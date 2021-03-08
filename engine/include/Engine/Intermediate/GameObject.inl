#include "Engine/Intermediate/GameObject.hpp"

GameObject::GameObject(Scene& scene, const CreateArg& arg)
    : m_name{arg.name}, m_transform{DataChunk<TransformComponent>::getInstance()->addComponent(*this,
                                                                                               arg.transformArg)},
      m_pComponents{}, scene{scene}, parent{arg.parent}

{
}

template <typename T>
void GameObject::updateComponentLink(const T* oldPtr, T* newPtr) noexcept
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
T& GameObject::addComponent(Args&&... args) noexcept
{
    T& newComponent = DataChunk<T>::getInstance()->addComponent(*this, std::forward<Args>(args)...);
    m_pComponents.emplace_back(&newComponent);
    return newComponent;
}

template <typename T>
T* GameObject::getComponent() noexcept
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

const std::string& GameObject::getName() const noexcept
{
    return m_name;
}

void GameObject::setName(const char* newName) noexcept
{
    m_name = newName;
}

constexpr const TransformComponent& GameObject::getTransform() const noexcept
{
    return m_transform;
}

constexpr TransformComponent& GameObject::getTransform() noexcept
{
    return m_transform;
}

template <typename T, typename... Args>
GameObject& GameObject::addChild(Args&&... args) noexcept
{
    std::unique_ptr<GameObject>& pChild = this->children.emplace_back(std::make_unique<T>(scene, args...));
    pChild->children                    = std::list<std::unique_ptr<GameObject>>();
    // pChild->update((*this).getModelMatrix());
    pChild->parent = this;
    return *pChild;
}

template <typename T>
std::vector<T*> GameObject::getComponents() noexcept
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

constexpr std::list<Component*>& GameObject::getComponents() noexcept
{
    return m_pComponents;
}

constexpr const std::list<Component*>& GameObject::getComponents() const noexcept
{
    return m_pComponents;
}

void GameObject::setTag(const std::string& newTag) noexcept
{
    m_tag = newTag;
}

constexpr const std::string& GameObject::getTag() const noexcept
{
    return m_tag;
}

std::list<std::unique_ptr<GameObject>>::iterator GameObject::destroyChild(
    const std::list<std::unique_ptr<GameObject>>::iterator& it) noexcept
{
    return children.erase(it);
}

template <typename TUniqueComponentType>
void GameObject::destroyUniqueComponentNow() noexcept
{
    for (auto&& it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
    {
        TUniqueComponentType* checkedCompPtr = dynamic_cast<TUniqueComponentType*>(*it);

        if (unlikely(checkedCompPtr != nullptr))
        {
            DataChunk<TUniqueComponentType>::getInstance()->destroyComponent(checkedCompPtr);
            m_pComponents.erase(it);
            return;
        }
    };
}

void GameObject::setActive(bool newState)
{
    for (auto&& i : m_pComponents)
    {
        i->setActive(newState);
    }
}

std::list<Component*>::iterator GameObject::destroyComponent(const std::list<Component*>::iterator& it) noexcept
{
    return m_pComponents.erase(it);
}

void GameObject::destroy() noexcept
{
    /*set flag to be delete by it parent*/
    m_isDead = true;
    // m_isDirty = true;
}

void GameObject::destroyNow() noexcept
{
    parent->destroyChild(this);
}

constexpr bool GameObject::operator==(GameObject const& other) noexcept
{
    return (this == &other);
}

bool GameObject::compareTag(const std::string& toCompare) const noexcept
{
    return (!toCompare.compare(m_tag));
}
