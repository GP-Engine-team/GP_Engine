#include "Engine/Intermediate/GameObject.hpp"

#include "Engine/Intermediate/DataChunk.hpp" //DataChunk
#include "Engine/Core/Debug/Assert.hpp" //GPE_ASSERT
#include "Engine/Core/Debug/Log.hpp"
#include <iostream>
#include <istream>
#include <sstream>

using namespace GPE;

void GameObject::updateSelfAndChildren() noexcept
{
    for (std::list<std::unique_ptr<GameObject>>::iterator i = children.begin(); i != children.end(); i++)
    {
        if ((*i)->m_transform.isDirty())
        {
            if ((*i)->m_isDead)
            {
                i = children.erase(i);
                continue;
            }

            (*i)->getTransform().update(m_transform.getModelMatrix());
            (*i)->forceUpdate();
        }
        else
        {
            (*i)->updateSelfAndChildren();
        }
    }
}

void GameObject::forceUpdate() noexcept
{
    for (auto&& i = children.begin(); i != children.end(); i++)
    {
        (*i)->getTransform().update(m_transform.getModelMatrix());
        (*i)->forceUpdate();
    }
}

GameObject* GameObject::getChild(const std::string& path) noexcept
{
    GPE_ASSERT(!path.empty(), "Void path");

    std::stringstream sPath(path);
    std::string word;
    GameObject* currentEntity = this;

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
                isFound = true;
                break;
            }
        }
        if (!isFound)
        {
            Log::logWarning(std::string("Can't find \"") + word + "\" in gameObject \"" +
                                                 m_name + "\"" + " with path : \"" + path + "\"");
            return nullptr;
        }
    }
    return currentEntity;
}

void GameObject::destroyChild(const std::string& path) noexcept
{
    GPE_ASSERT(!path.empty(), "Void path");

    std::stringstream sPath(path);
    std::string word;
    GameObject* parentEntity = this;
    GameObject* currentEntity = this;
    std::list<std::unique_ptr<GameObject>>::iterator it;

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
                isFound = true;
                break;
            }
        }

        if (!isFound)
        {
            Log::logWarning(std::string("Canno't found \"") + word + "\" in gameObject \"" +
                                                 m_name + "\"" + " with path : \"" + path + "\"");
            return;
        }
    }

    parentEntity->children.erase(it);
}

std::list<std::unique_ptr<GameObject>>::iterator GameObject::destroyChild(GameObject* pGameObject) noexcept
{
    for (std::list<std::unique_ptr<GameObject>>::iterator it = children.begin(); it != children.end(); it++)
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
    std::string path = this->getName();
    GameObject* parentIt = this->parent;

    while (parentIt)
    {
        path = parentIt->getName() + std::string("/") + path;
        parentIt = parentIt->parent;
    }

    return path;
}