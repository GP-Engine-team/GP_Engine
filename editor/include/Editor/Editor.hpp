/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

struct GLFWwindow;

namespace Editor
{

class Editor
{
private:
	GLFWwindow* m_window;
	int			m_framebufferWidth;
	int			m_framebufferHeight;

	void initGLFW();
	void initGlad();
	void initDearImGui();

public:
	Editor();
	~Editor();

	void update		(double unscaledDeltaTime, double deltaTime);
	void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime);
	void render		();
	bool isRunning  ();

	friend void windowFramebufferResized(GLFWwindow* window, int width, int height);
};

} // End of namespace Editor