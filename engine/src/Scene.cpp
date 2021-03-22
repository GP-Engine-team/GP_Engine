#include "Engine/Resources/Scene.hpp"

#include "Engine/Core/Debug/Assert.hpp"

using namespace GPE;

GameObject* Scene::getGameObject(const std::string& path) noexcept
{
    GPE_ASSERT(!path.empty(), "Empty path");

    std::stringstream sPath(path);
    std::string       word;
    GameObject*       currentEntity = &world;

    while (std::getline(sPath, word, '/'))
    {
        if (word.empty() || word == "." || word == "world")
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
            Log::getInstance()->logError(std::string("Canno't found \"") + word + "\" in scene graph \"" + path + "\"");
            return nullptr;
        }
    }
    return currentEntity;
}