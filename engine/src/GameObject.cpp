#include "Engine/Core/Debug/Assert.hpp" //GPE_ASSERT
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Intermediate/DataChunk.hpp" //DataChunk
#include "imgui.h"
#include <istream>
#include <sstream>

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"

// Generated
#include "Generated/GameObject.rfk.h"
File_GENERATED

    using namespace GPE;
using namespace GPM;

unsigned int GameObject::m_currentID = 0;

GameObject::~GameObject() noexcept
{
    m_pTransform->destroy();

    for (GameObject* child : children)
    {
        delete child;
    }

    for (auto&& component : m_pComponents)
    {
        delete component;
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
    const Children::const_iterator end{children.end()};

    if (m_pTransform->isDirty())
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
            getTransform().update();
        }

        // Update children
        for (Children::iterator i = children.begin(); i != end; i++)
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
        for (Children::iterator i = children.begin(); i != end;)
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
    const Children::const_iterator end{children.end()};
    for (Children::iterator i = children.begin(); i != end;)
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
        getTransform().update();
    }

    // Force update children
    const Children::const_iterator end{children.end()};
    for (auto&& i = children.begin(); i != end; i++)
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
    const Children::const_iterator end{children.end()};
    for (auto&& i = children.begin(); i != end; i++)
    {
        (*i)->forceUpdate(m_pTransform->getModelMatrix());
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

    std::stringstream  sPath(path);
    std::string        word;
    GameObject*        parentEntity  = this;
    GameObject*        currentEntity = this;
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

GameObject::Children::iterator GameObject::destroyChild(GameObject* pGameObject) noexcept
{
    const Children::const_iterator end{children.end()};
    for (Children::iterator it = children.begin(); it != end; it++)
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

void GameObject::detach(const GameObject::Children::iterator& itToParentPtr) noexcept
{
    m_parent->children.erase(itToParentPtr);
    m_parent    = nullptr;
    pOwnerScene = nullptr;
}

void GameObject::inspect(GPE::InspectContext& context)
{
    GPE::DataInspector::inspect(context, m_name, "name");

    getTransform().inspect(context);

    std::list<Component*>& comps = getComponents();

    for (auto&& it = comps.begin(); it != comps.end();)
    {
        ImGui::PushID(&*it);

        const bool isCollapsingHOpen = ImGui::CollapsingHeader((*it)->getArchetype().name.c_str());

        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("GameObjectContextePopup");
        }

        bool isItCanIterator = true;
        if (ImGui::BeginPopup("GameObjectContextePopup"))
        {
            if (ImGui::MenuItem("Remove component", NULL, false))
            {
                it              = destroyComponent(*it);
                isItCanIterator = false;
            }

            ImGui::EndPopup();
        }

        if (isCollapsingHOpen)
            GPE::DataInspector::inspect(context, **it);

        ImGui::PopID();
        if (isItCanIterator)
            ++it;
    }
}

void GPE::save(XmlSaver& context, GameObject& inspected)
{
    const rfk::Class& archetype = GameObject::staticGetArchetype();

    // TODO : Replace "gameObject" by unique name.
    context.push("gameObject", archetype.name, archetype.id);

    inspected.save(context);

    context.pop();
}

void GPE::load(XmlLoader& context, class GameObject& inspected)
{
    const rfk::Class& archetype = GameObject::staticGetArchetype();

    // TODO : Replace "gameObject" by unique name.
    XmlLoader::LoadInfo info{"gameObject", archetype.name, archetype.id};
    if (context.goToSubChild(info))
    {
        inspected.load(context);
        context.pop();
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

void* GameObject::operator new(std::size_t size)
{
    return GPE::DataChunk<GameObject>::getInstance()->add();
}

void GameObject::operator delete(void* ptr)
{
    GPE::DataChunk<GameObject>::getInstance()->destroy(static_cast<GameObject*>(ptr));
}