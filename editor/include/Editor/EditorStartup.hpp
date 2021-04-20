﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Core/Game/ContextStartup.hpp>
#include <Engine/Core/HotReload/ReloadableCpp.hpp>

#include "Editor.hpp"

#include <functional>

struct GLFWwindow;

namespace Editor
{
class EditorStartup final : public ContextStartup
{
private:
	std::function<void(double, double)> m_fixedUpdate;
	std::function<void(double, double)> m_update;
	const std::function<void()>			m_render;

#ifdef NDEBUG
    const char* gameDllPath = "./bin/Release/GPGame.dll";
#else
    const char* gameDllPath = "./bin/Debug/GPGame.dll";
#endif

	Editor			   m_editor;
	GPE::ReloadableCpp m_reloadableCpp;
	GPE::AbstractGame* m_game;

protected:
    GLFWwindow* initDearImGui(GLFWwindow* window);

	void initializeDefaultInputs() const;

public:
    EditorStartup();
    virtual ~EditorStartup() final;

	void openGame ();
	void closeGame();

	void playGame ();
	void pauseGame();
	void stopGame ();

	GPE::AbstractGame& game() const;

	virtual void update() override final;
};

} // End of namespace Editor
