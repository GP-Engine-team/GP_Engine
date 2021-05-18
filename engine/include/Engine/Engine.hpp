/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Core/Rendering/Window/WindowGLFW.hpp"
#include "ECS/System/InputManagerGLFW.hpp"
#include "ECS/System/PhysXSystem.hpp"
#include "ECS/System/SoundSystem.hpp"
#include "ECS/System/TimeSystem.hpp"
#include "ECS/System/AnimationSystem.hpp"
#include "Resources/ResourcesManagerType.hpp"
#include "Resources/SceneManager.hpp"

namespace GPE
{

/*
 * The Engine class defines the `getInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class Engine
{
protected:
    static Engine* m_instance;

public:
    Window              window;
    Renderer            renderer;
    TimeSystem          timeSystem;
    InputManager        inputManager;
    ResourceManagerType resourceManager;
    SceneManager        sceneManager;
    PhysXSystem         physXSystem;    
    SoundSystem         soundSystem;
    AnimationSystem     animSystem;
    std::function<void()> exit;

protected:
    Engine()
        : window{Window::CreateArg{"Window", 900, 600}}, renderer{window}, timeSystem{}, inputManager{window},
          resourceManager{}, sceneManager{}, physXSystem{}, soundSystem{},
          exit{[&]()
          {
              window.close();
          }}
    {
    }

public:
    /**
     * Singletons should not be cloneable.
     */
    Engine(Engine& other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const Engine&) = delete;

    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static Engine* getInstance()
    {
        if (m_instance == nullptr)
        {
            m_instance = new Engine();
        }

        return m_instance;
    }

    static void setInstance(Engine& engine)
    {
        m_instance = &engine;
    }

    void destroy()
    {
        delete (m_instance);
    }
};

} /*namespace GPE*/
