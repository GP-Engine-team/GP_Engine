/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Input/InputManagerGLFW.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/TimeSystem/TimeSystem.hpp"
#include "Engine/Intermediate/BehaviourSystem.hpp"
#include "Engine/Intermediate/RenderSystem.hpp"

namespace GPE
{

/**
 * The SystemsManager class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class SystemsManager
{
public:
    Window          window;
    Renderer        renderer;
    TimeSystem      timeSystem;
    InputManager    inputManager;
    BehaviourSystem behaviourSystem;
    RenderSystem    renderSystem;

protected:
    SystemsManager()
        : window{Window::CreateArg{"window", 800, 900}}, renderer{window}, timeSystem{}, inputManager{window},
          behaviourSystem{}, renderSystem{}
    {
    }

    static SystemsManager* m_instance;

public:
    /**
     * Singletons should not be cloneable.
     */
    SystemsManager(SystemsManager& other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const SystemsManager&) = delete;

    SystemsManager(SystemsManager&& other) noexcept = delete;
    void operator=(SystemsManager&& other) noexcept = delete;

    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static SystemsManager* getInstance();
};

SystemsManager* SystemsManager::m_instance = nullptr;
;

/**
 * Static methods should be defined outside the class.
 */
SystemsManager* SystemsManager::getInstance()
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if (m_instance == nullptr)
    {
        m_instance = new SystemsManager();
    }
    return m_instance;
}

} /*namespace GPE*/
