/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"

namespace Editor
{

class Editor
{
private:
	Engine::Core::Renderering::Window window;

public:
	Editor();
	~Editor();

	void update		(double unscaledDeltaTime, double deltaTime);
	void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime);
	void render		();
};

} // End of namespace Editor