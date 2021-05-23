#pragma once

// You should only include this file in .cpp files.

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Engine.hpp"
#include "EngineApiMacros.hpp"

#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/DataInspector.hpp"
#include "Engine/Serialization/InspectContext.hpp"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <memory>

struct ImGuiContext;

extern "C"
{
    ENGINE_API inline void setGameEngineInstance(GPE::Engine& engine)
    {
        GPE::Engine::setInstance(engine);
    }
    ENGINE_API inline void setLogInstance(GPE::Log& log)
    {
        GPE::Log::setInstance(log);
    }
    ENGINE_API inline GPE::Engine& getGameEngineInstance()
    {
        return *GPE::Engine::getInstance();
    }
    ENGINE_API inline GPE::Log& getLogInstance()
    {
        return *GPE::Log::getInstance();
    }
    ENGINE_API inline void setImguiCurrentContext(struct ImGuiContext* ctx)
    {
        ImGui::SetCurrentContext(ctx);
    }
    ENGINE_API inline ImGuiContext* getGameUIContext()
    {
        return ImGui::GetCurrentContext();
    }

    ENGINE_API inline void saveSceneToPath(GPE::Scene* scene, const char* path, GPE::SavedScene::EType saveMode)
    {
        GPE::saveSceneToPathImp(scene, path, saveMode);
    }

    ENGINE_API inline void loadSceneFromPath(GPE::Scene* scene, const char* path)
    {
        GPE::loadSceneFromPathImp(scene, path);
    }

    ENGINE_API inline GPE::GameObject* loadPrefabFromPath(GPE::GameObject& parent, const char* path)
    {
        return GPE::loadPrefabFromPathImp(parent, path);
    }

    ENGINE_API class GPE::AbstractGame* createGameInstance();
    ENGINE_API void                     destroyGameInstance(class GPE::AbstractGame* game);

#include <Refureku/Refureku.h>

    ENGINE_API inline void getAllComponentsName(std::vector<std::string>* out)
    {
        assert(out != nullptr);

        const std::unordered_set<rfk::Archetype const*, rfk::Entity::PtrNameHasher, rfk::Entity::PtrEqualName>&
            engineArchetypes = rfk::Database::getNamespace("GPE")->archetypes;

        // Get names of components of GPEngine
        for (rfk::Archetype const* archetype : engineArchetypes)
        {
            if (archetype->getProperty<ComponentGen>())
            {
                out->push_back(archetype->name);
            }
        }

        const std::unordered_set<rfk::Archetype const*, rfk::Entity::PtrNameHasher, rfk::Entity::PtrEqualName>&
            gameArchetypes = rfk::Database::getNamespace("GPG")->archetypes;

        // Get names of components of GPGame
        for (rfk::Archetype const* archetype : gameArchetypes)
        {
            if (archetype->getProperty<ComponentGen>())
            {
                out->push_back(archetype->name);
            }
        }
    }

    ENGINE_API inline void getAllComponentsClasses(std::vector<rfk::Archetype const*>* out)
    {
        assert(out != nullptr);

        const std::unordered_set<rfk::Archetype const*, rfk::Entity::PtrNameHasher, rfk::Entity::PtrEqualName>&
            engineArchetypes = rfk::Database::getNamespace("GPE")->archetypes;

        // Get names of components of GPEngine
        for (rfk::Archetype const* archetype : engineArchetypes)
        {
            if (archetype->getProperty<ComponentGen>())
            {
                out->emplace_back(archetype);
            }
        }

        const std::unordered_set<rfk::Archetype const*, rfk::Entity::PtrNameHasher, rfk::Entity::PtrEqualName>&
            gameArchetypes = rfk::Database::getNamespace("GPG")->archetypes;

        // Get names of components of GPGame
        for (rfk::Archetype const* archetype : gameArchetypes)
        {
            if (archetype->getProperty<ComponentGen>())
            {
                out->emplace_back(archetype);
            }
        }
    }

    ENGINE_API inline const rfk::Class& getComponentClass()
    {
        return GPE::Component::staticGetArchetype();
    }

    ENGINE_API inline GPE::GameObject* createGameObject()
    {
        return new GPE::GameObject();
    }

    ENGINE_API inline void destroyGameObject(GPE::GameObject* gameObject)
    {
        assert(gameObject != nullptr);
        delete gameObject;
    }

    ENGINE_API inline GPE::Component* createComponentByName(const std::string& name)
    {
        return rfk::Database::getClass(name)->makeInstance<GPE::Component>();
    }

    ENGINE_API inline GPE::Component* createComponentByID(GPE::GameObject& gameObject, const rfk::uint64 compID)
    {
        rfk::Class const* archetype = static_cast<rfk::Class const*>(rfk::Database::getEntity(compID));
        GPE::Component*   newComp   = archetype->makeInstance<GPE::Component>();
        newComp->setOwner(gameObject);
        newComp->onPostLoad();
        return gameObject.addExistingComponent(newComp);
    }

    ENGINE_API inline void destroyComponent(GPE::GameObject* gameObject)
    {
        assert(gameObject != nullptr);
        delete gameObject;
    }
}