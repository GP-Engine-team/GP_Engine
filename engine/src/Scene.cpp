#include "Engine/Resources/Scene.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Intermediate/DataChunk.hpp"
#include "Engine/Resources/Importer/ResourceImporter.hpp"

#include <filesystem> //std::path
#include <sstream>    //std::sstream, std::getline

using namespace GPE;

Scene::Scene() noexcept : m_pWorld(&DataChunk<GameObject>::getInstance()->add(*this))
{
    for (auto&& elem : m_loadedResourcesPath)
    {
        // importeResource(elem.first.c_str());
    }
}

Scene::~Scene() noexcept
{
    // DataChunk<GameObject>::getInstance()->destroy(m_pWorld);
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