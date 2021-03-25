/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <imgui/imgui.h>

namespace Editor
{
	class LogInspector
	{
	protected:
		ImGuiTextBuffer m_buf;
		ImGuiTextFilter m_filter;
		ImVector<int>   m_lineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		bool            m_autoScroll;  // Keep scrolling if already at the bottom.

	public:
		LogInspector();

		~LogInspector();

		void clear();

		void addLog(const char* fmt, ...) IM_FMTARGS(2);

		void draw(const char* title, bool* p_open = NULL);
	};
} // End of namespace Editor
