#include "Engine/Intermediate/GameObject.hpp"

#include "Engine/Core/Debug/Assert.hpp" //GPE_ASSERT
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Intermediate/DataChunk.hpp" //DataChunk
#include <iostream>
#include <istream>
#include <sstream>
#include "imgui.h"

using namespace GPE;
using namespace GPM;

GameObject::~GameObject() noexcept
{
    DataChunk<TransformComponent>::getInstance()->destroy(&m_transform);

    /*
    for (auto&& component : m_pComponents)
    {
        // component->destroy();
    }
    */
}

void GameObject::moveTowardScene(Scene& newOwner) noexcept
{
    for (Component* pComponent : m_pComponents)
    {
        pComponent->moveTowardScene(newOwner);
    }

    pOwnerScene = &newOwner;
}

void GameObject::updateSelfAndChildren() noexcept
{
    const Children::const_iterator end{children.end()};

    if (m_transform.isDirty())
    {
        // Update self
        if (m_parent)
        {
            getTransform().update(m_parent->getTransform().getModelMatrix());

            if (m_isDead)
            {
                // NOTE (Sami): what happens to the children?
                m_parent->destroyChild(this);
                return;
            }
        }
        else
        {
            getTransform().update(Mat4::identity());
        }

        // Update children
        for (Children::iterator i = children.begin(); i != end; i++)
        {
            if ((*i)->m_isDead)
            {
                i = children.erase(i);
                continue;
            }

            (*i)->forceUpdate(m_transform.getModelMatrix());
        }
    }
    else
    {
        // Update children
        for (Children::iterator i = children.begin(); i != end; i++)
        {
            if ((*i)->m_isDead)
            {
                i = children.erase(i);
                continue;
            }

            (*i)->updateSelfAndChildren(m_transform.getModelMatrix());
        }
    }
}

void GameObject::updateSelfAndChildren(const Mat4 parentModelMatrix) noexcept
{
    // Update self
    if (m_transform.isDirty())
        getTransform().update(m_parent->getTransform().getModelMatrix());

    // Update children
    const Children::const_iterator end{children.end()};
    for (Children::iterator i = children.begin(); i != end;)
    {
        if ((*i)->m_isDead)
        {
            i = children.erase(i);
            continue;
        }

        if ((*i)->m_transform.isDirty())
        {
            (*i)->forceUpdate(m_transform.getModelMatrix());
        }
        else
        {
            (*i)->updateSelfAndChildren(m_transform.getModelMatrix());
        }
        ++i;
    }
}

void GameObject::forceUpdate() noexcept
{
    // Force update self
    if (m_parent)
    {
        getTransform().update(m_parent->getTransform().getModelMatrix());
    }
    else
    {
        getTransform().update(Mat4::identity());
    }

    // Force update children
    const Children::const_iterator end{children.end()};
    for (auto&& i = children.begin(); i != end; i++)
    {
        if ((*i)->m_isDead)
        {
            i = children.erase(i);
            continue;
        }

        (*i)->forceUpdate(m_transform.getModelMatrix());
    }
}

void GameObject::forceUpdate(const GPM::Mat4 parentModelMatrix) noexcept
{
    // Force update self
    getTransform().update(m_parent->getTransform().getModelMatrix());

    // Force update children
    const Children::const_iterator end{children.end()};
    for (auto&& i = children.begin(); i != end; i++)
    {
        (*i)->forceUpdate(m_transform.getModelMatrix());
    }
}

GameObject* GameObject::getChild(const std::string& path) noexcept
{
    if (path.empty())
    {
        Log::getInstance()->logWarning("The passed string is empty");
        return nullptr;
    }

    std::stringstream sPath(path);
    std::string       word;
    GameObject*       currentEntity = this;

    while (std::getline(sPath, word, '/'))
    {
        if (word.empty() || word == "." || word == m_name)
            continue;

        bool isFound = false;
        for (auto&& child : currentEntity->children)
        {
            if (child->getName() == word)
            {
                currentEntity = child.get();
                isFound       = true;
                break;
            }
        }
        if (!isFound)
        {
            Log::getInstance()->logWarning(std::string("Can't find \"") + word + "\" in gameObject \"" + m_name +
                                           "\"" + " with path : \"" + path + "\"");
            return nullptr;
        }
    }
    return currentEntity;
}

void GameObject::destroyChild(const std::string& path) noexcept
{
    GPE_ASSERT(!path.empty(), "Void path");

    std::stringstream                                sPath(path);
    std::string                                      word;
    GameObject*                                      parentEntity  = this;
    GameObject*                                      currentEntity = this;
    Children::iterator it;

    while (std::getline(sPath, word, '/'))
    {
        if (word.empty() || word == "." || word == m_name)
            continue;

        bool isFound = false;
        parentEntity = currentEntity;

        for (it = parentEntity->children.begin(); it != parentEntity->children.end(); it++)
        {
            if ((*it)->getName() == word)
            {
                currentEntity = it->get();
                isFound       = true;
                break;
            }
        }

        if (!isFound)
        {
            Log::getInstance()->logWarning(std::string("Canno't found \"") + word + "\" in gameObject \"" + m_name +
                                           "\"" + " with path : \"" + path + "\"");
            return;
        }
    }

    parentEntity->children.erase(it);
}

GameObject::Children::iterator GameObject::destroyChild(GameObject* pGameObject) noexcept
{
    const Children::const_iterator end{children.end()};
    for (Children::iterator it = children.begin(); it != end; it++)
    {
        if ((*it).get() == pGameObject)
        {
            return children.erase(it);
        }
    }
    return children.end();
}

std::list<Component*>::iterator GameObject::destroyComponent(Component* pComponent) noexcept
{
    const std::list<Component*>::const_iterator end{m_pComponents.end()};
    for (std::list<Component*>::iterator it = m_pComponents.begin(); it != end; it++)
    {
        if (*it == pComponent)
        {
            return m_pComponents.erase(it);
        }
    }
    return m_pComponents.end();
}

std::string GameObject::getAbsolutePath() const noexcept
{
    std::string path     = this->getName();
    GameObject* parentIt = this->m_parent;

    while (parentIt)
    {
        path     = parentIt->getName() + std::string("/") + path;
        parentIt = parentIt->m_parent;
    }

    return path;
}

template <>
static void GPE::DataInspector::inspect(class GameObject& inspected)
{
    inspected.inspect();

    std::list<Component*>& comps = inspected.getComponents();
    unsigned int           i     = 0;
    for (Component* comp : comps)
    {
        ImGui::PushID(i);
        comp->inspect();
        ImGui::PopID();
        i++;
    }
}