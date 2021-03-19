/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/ECS/System/PhysXSystem.hpp"
#include "Engine/ECS/System/RenderSystem.hpp"
#include "Engine/ECS/System/TimeSystem.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/SceneManager.hpp"

namespace GPE
{

/**
 * The SystemsManager class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class SystemsManager
{
protected:
    static SystemsManager* m_instance;

public:
    Window              window;
    Renderer            renderer;
    TimeSystem          timeSystem;
    InputManager        inputManager;
    BehaviourSystem     behaviourSystem;
    ResourceManagerType resourceManager;
    RenderSystem        renderSystem;
    SceneManager        sceneManager;
    PhysXSystem         physXSystem;

protected:
    SystemsManager()
        : window{Window::CreateArg{"window", 900, 600}}, renderer{window}, timeSystem{}, inputManager{window},
          behaviourSystem{}, resourceManager{}, renderSystem{}, sceneManager{}, physXSystem{}
    {
        renderSystem.addRenderer(&renderer);
    }

public:
    /**
     * Singletons should not be cloneable.
     */
    SystemsManager(SystemsManager& other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const SystemsManager&) = delete;

    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static SystemsManager* getInstance()
    {
        if (m_instance == nullptr)
        {
            m_instance = new SystemsManager();
        }

        return m_instance;
    }

    static void setInstance(SystemsManager& systemManager)
    {
        m_instance = &systemManager;
    }
};

} /*namespace GPE*/
