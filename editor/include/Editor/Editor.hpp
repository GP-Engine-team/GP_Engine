/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"

namespace Editor
{

class Editor
{
private:
	Engine::Core::Renderering::Window   window;
	Engine::Core::Renderering::Renderer renderer;

public:
	Editor();
	~Editor();

	void update		(double unscaledDeltaTime, double deltaTime);
	void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime);
	void render		();
	bool isRunning  ();
};

} // End of namespace Editor