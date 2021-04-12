#include "Engine/Intermediate/GameObject.hpp"

#include "Engine/Core/Debug/Assert.hpp" //GPE_ASSERT
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Intermediate/DataChunk.hpp" //DataChunk
#include "imgui.h"
#include <iostream>
#include <istream>
#include <sstream>

using namespace GPE;
using namespace GPM;

unsigned int GameObject::m_currentID = 0;

GameObject::~GameObject() noexcept
{
    m_pTransform->destroy();

    for (auto&& component : m_pComponents)
    {
        component->destroy();
    }
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
    if (m_pTransform->isDirty())
    {
        // Update self
        if (m_parent)
        {
            getTransform().update(m_parent->getTransform().getModelMatrix());

            if (m_isDead)
            {
                m_parent->destroyChild(this);
                return;
            }
        }
        else
        {
            getTransform().update(Mat4::identity());
        }

        // Update children
        for (std::list<GameObject*>::iterator i = children.begin(); i != children.end();)
        {
            if ((*i)->m_isDead)
            {
                i = destroyChild(i);
                continue;
            }

            (*i)->forceUpdate(m_pTransform->getModelMatrix());
            ++i;
        }
    }
    else
    {
        // Update children
        for (std::list<GameObject*>::iterator i = children.begin(); i != children.end();)
        {
            if ((*i)->m_isDead)
            {
                i = destroyChild(i);
            }
            else
            {
                (*i)->updateSelfAndChildren(m_pTransform->getModelMatrix());
                ++i;
            }
        }
    }
}

void GameObject::updateSelfAndChildren(const Mat4 parentModelMatrix) noexcept
{
    // Update self
    if (m_pTransform->isDirty())
        getTransform().update(m_parent->getTransform().getModelMatrix());

    // Update children
    for (std::list<GameObject*>::iterator i = children.begin(); i != children.end();)
    {
        if ((*i)->m_isDead)
        {
            i = destroyChild(i);
        }
        else
        {
            if ((*i)->m_pTransform->isDirty())
            {
                (*i)->forceUpdate(m_pTransform->getModelMatrix());
            }
            else
            {
                (*i)->updateSelfAndChildren(m_pTransform->getModelMatrix());
            }
            ++i;
        }
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
    for (auto&& i = children.begin(); i != children.end();)
    {
        if ((*i)->m_isDead)
        {
            i = destroyChild(i);
            continue;
        }

        (*i)->forceUpdate(m_pTransform->getModelMatrix());
        ++i;
    }
}

void GameObject::forceUpdate(const GPM::Mat4 parentModelMatrix) noexcept
{
    // Force update self
    getTransform().update(m_parent->getTransform().getModelMatrix());

    // Force update children
    for (auto&& i = children.begin(); i != children.end(); i++)
    {
        (*i)->forceUpdate(m_pTransform->getModelMatrix());
    }
}

GameObject* GameObject::getChild(const std::string& path) noexcept
{
    GPE_ASSERT(!path.empty(), "Void path");

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
                currentEntity = child;
                isFound       = true;
                break;
            }
        }
        if (!isFound)
        {
            Log::getInstance()->logWarning(std::string("Can't find \"") + word + "\" in gameObject \"" + m_name + "\"" +
                                           " with path : \"" + path + "\"");
            return nullptr;
        }
    }
    return currentEntity;
}

void GameObject::destroyChild(const std::string& path) noexcept
{
    GPE_ASSERT(!path.empty(), "Void path");

    std::stringstream                sPath(path);
    std::string                      word;
    GameObject*                      parentEntity  = this;
    GameObject*                      currentEntity = this;
    std::list<GameObject*>::iterator it;

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
                currentEntity = *it;
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

std::list<GameObject*>::iterator GameObject::destroyChild(GameObject* pGameObject) noexcept
{
    for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
    {
        if (*it == pGameObject)
        {
            return destroyChild(it);
        }
    }
    return children.end();
}

std::list<Component*>::iterator GameObject::destroyComponent(Component* pComponent) noexcept
{
    for (std::list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); it++)
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

void GameObject::inspect(GPE::InspectContext& context)
{
    GPE::DataInspector::inspect(context, m_name, "name");

    getTransform().inspect(context);

    std::list<Component*>& comps = getComponents();

    for (Component* comp : comps)
    {
        ImGui::PushID(comp);
        // comp->inspect();
        GPE::DataInspector::inspect(context, *comp);
        ImGui::PopID();
    }
}

GameObject* GameObject::getGameObjectCorrespondingToID(unsigned int ID) noexcept
{
    if (m_id == ID)
        return this;

    GameObject* rst = nullptr;

    for (auto&& child : children)
    {
        if (rst = child->getGameObjectCorrespondingToID(ID))
        {
            return rst;
        }
    }

    return nullptr;
}