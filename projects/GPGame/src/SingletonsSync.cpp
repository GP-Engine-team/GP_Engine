#include "SingletonsSync.hpp"

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
#include <Windows.h>
#include <memory>

extern "C" void setLogInstance(GPE::Log& log)
{
    GPE::Log::setInstance(log);
}

extern "C" GPE::Log& getLogInstance()
{
    return *GPE::Log::getInstance();
}

extern "C" void setGameEngineInstance(GPE::Engine& engine)
{
    GPE::Engine::setInstance(engine);
}

extern "C" GPE::Engine& getGameEngineInstance()
{
    return *GPE::Engine::getInstance();
}

extern "C" void setImguiCurrentContext(ImGuiContext* ctx)
{
    ImGui::SetCurrentContext(ctx);
}

extern "C" ImGuiContext* getGameUIContext()
{
    return ImGui::GetCurrentContext();
}

extern "C" void saveScene(XmlSaver& context, GPE::Scene* scene)
{
    // GPE::Engine::getInstance()->sceneManager.getCurrentScene()->save(context);
    context.addWeakPtr(scene);
    scene->save(context);
}

extern "C" void loadScene(XmlLoader& context, GPE::Scene* scene)
{
    context.addPersistentPtr(scene);

    // GPE::Engine::getInstance()->sceneManager.getCurrentScene()->load(context);
    scene->load(context);
    context.updateLazyPtrs();
}

extern "C" void saveSceneToPath(GPE::Scene* scene, const char* path, GPE::SavedScene::EType saveMode)
{
    if (saveMode == GPE::SavedScene::EType::XML)
    {
        rapidxml::xml_document<> doc;
        XmlSaver                 context(doc);
        context.addWeakPtr(scene);
        scene->save(context);

        GPE::SavedScene::CreateArg args;
        args.data = docToString(doc);
        args.type = GPE::SavedScene::EType::XML;

        GPE::writeSceneFile(path, args);
    }
}

extern "C" void loadSceneFromPath(GPE::Scene* scene, const char* path)
{
    GPE::SavedScene::CreateArg savedScene = GPE::readSceneFile(path);

    if (savedScene.type == GPE::SavedScene::EType::XML)
    {
        // Load xml doc
        rapidxml::xml_document<> doc;
        std::unique_ptr<char[]>  buffer;
        GPE::SavedScene::toDoc(doc, buffer, savedScene.data.c_str(), savedScene.data.size());

        XmlLoader context(doc);
        // Load each element
        scene->load(context);

        // Tell that pointers to the old scene should be replaced by pointers to the new scene
        context.addConvertedPtr(scene->getWorld().pOwnerScene, scene);

        // Update old pointers into new ones
        context.updateLazyPtrs();

        // Awake GameObjects
        struct Rec
        {
            static void rec(GPE::GameObject* g)
            {
                for (GPE::Component* comp : g->getComponents())
                {
                    comp->awake();
                }

                g->getTransform().awake();

                for (GPE::GameObject* g2 : g->children)
                {
                    rec(g2);
                }
            };
        };
        Rec::rec(&scene->getWorld()); // can't do recursives with lambdas, and std::function would be overkill
    }
}