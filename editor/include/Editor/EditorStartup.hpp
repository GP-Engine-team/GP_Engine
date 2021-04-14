/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Editor.hpp"
#include "Editor/Editor.hpp"
#include "Engine/Core/Game/ContextStartup.hpp"
#include "Engine/Core/HotReload/ReloadableCpp.hpp"

#include <functional>

class AbstractGame;
struct GLFWwindow;

namespace Editor
{
class EditorStartup final : public ContextStartup
{
private:
    const std::function<void(double, double)> m_fixedUpdate;
    const std::function<void(double, double)> m_update;
    const std::function<void()>               m_render;

#ifdef NDEBUG
    const char* gameDllPath = "./bin/Release/GPGame.dll";
#else
    const char* gameDllPath = "./bin/Debug/GPGame.dll";
#endif

    GPE::ReloadableCpp m_reloadableCpp;
    Editor             m_editor;
    AbstractGame*      m_game;

protected:
    GLFWwindow* initDearImGui(GLFWwindow* window);

public:
    EditorStartup();
    virtual ~EditorStartup() final;

    void         startGame();
    void         closeGame();
    virtual void update() override final;
};

} // End of namespace Editor
