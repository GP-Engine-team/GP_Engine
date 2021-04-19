#include "Engine/Resources/Scene.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Intermediate/DataChunk.hpp"
#include "Engine/Resources/Importer/ResourceImporter.hpp"

#include <filesystem> //std::path
#include <sstream>    //std::sstream, std::getline

#include "Engine/Serialization/xml/xmlLoader.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"

using namespace GPE;

Scene::Scene() noexcept : m_pWorld(new GameObject(*this))
{
    for (auto&& elem : m_loadedResourcesPath)
    {
        // importeResource(elem.first.c_str());
    }
}

GameObject* Scene::getGameObject(const std::string& path) noexcept
{
    GPE_ASSERT(!path.empty(), "Empty path");

    std::stringstream sPath(path);
    std::string       word;
    GameObject*       currentEntity = m_pWorld;

    while (std::getline(sPath, word, '/'))
    {
        if (word.empty() || word == "." || word == "world")
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
            Log::getInstance()->logError(std::string("Canno't found \"") + word + "\" in scene graph \"" + path + "\"");
            return nullptr;
        }
    }
    return currentEntity;
}

GameObject& Scene::getWorld() noexcept
{
    return *m_pWorld;
}

void Scene::addLoadedResourcePath(const char* path) noexcept
{
    // Unordered pair of iterator and result
    auto itRst = m_loadedResourcesPath.try_emplace(path, 1);

    if (itRst.second)
    {
        // importeResource(path);
        Log::getInstance()->log(stringFormat("Resource add to scene \"%s\" with path : %s", m_name.c_str(), path));
    }
    else
    {
        itRst.first->second++;
    }
}

void Scene::removeLoadedResourcePath(const char* path) noexcept
{
    if (--m_loadedResourcesPath[path] == 0)
    {
        m_loadedResourcesPath.erase(path);
        Log::getInstance()->log(stringFormat("Resource remove from scene \"%s\" with path : %s", m_name.c_str(), path));
    }
}

void Scene::save(XmlSaver& context) const
{
    GPE::save(context, m_pWorld, XmlSaver::SaveInfo{"m_pWorld", "GameObject", GameObject::staticGetArchetype().id});
}

void Scene::load(XmlLoader& context)
{
    delete m_pWorld;
    m_pWorld = nullptr;
    GPE::load(context, m_pWorld, XmlLoader::LoadInfo{"m_pWorld", "GameObject", GameObject::staticGetArchetype().id});
}