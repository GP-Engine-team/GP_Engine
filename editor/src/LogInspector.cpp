#include "Editor/LogInspector.hpp"

#include "Engine/Core//Debug/Log.hpp"

namespace Editor
{

	LogInspector::LogInspector()
	{
		m_autoScroll = true;
		clear();

		for (auto&& str : GPE::Log::getInstance()->getLogs())
		{
			std::cout << str;
			addLog(str.c_str());
		}
	}

	LogInspector::~LogInspector()
	{
		clear();
	}

	void LogInspector::clear()
	{
		m_buf.clear();
		m_lineOffsets.clear();
		m_lineOffsets.push_back(0);
	}

	void LogInspector::addLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int     old_size = m_buf.size();
		va_list args;
		va_start(args, fmt);
		m_buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = m_buf.size(); old_size < new_size; old_size++)
			if (m_buf[old_size] == '\n')
				m_lineOffsets.push_back(old_size + 1);
	}

	void LogInspector::draw(const char* title, bool* p_open)
	{
		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &m_autoScroll);
			ImGui::EndPopup();
		}

		// Main window
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		bool isClear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool isCopy = ImGui::Button("Copy");
		ImGui::SameLine();
		m_filter.Draw("Filter", -100.0f);

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (isClear)
			clear();

		if (isCopy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = m_buf.begin();
		const char* buf_end = m_buf.end();
		if (m_filter.IsActive())
		{
			// In this example we don't use the clipper when m_filter is enabled.
			// This is because we don't have a random access on the result on our filter.
			// A real application processing logs with ten of thousands of entries may want to store the result of
			// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
			for (int line_no = 0; line_no < m_lineOffsets.Size; line_no++)
			{
				const char* line_start = buf + m_lineOffsets[line_no];
				const char* line_end =
					(line_no + 1 < m_lineOffsets.Size) ? (buf + m_lineOffsets[line_no + 1] - 1) : buf_end;
				if (m_filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			// The simplest and easy way to display the entire buffer:
			//   ImGui::TextUnformatted(buf_begin, buf_end);
			// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
			// to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
			// within the visible area.
			// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
			// on your side is recommended. Using ImGuiListClipper requires
			// - A) random access into your data
			// - B) items all being the  same height,
			// both of which we can handle since we an array pointing to the beginning of each line of text.
			// When using the filter (in the block of code above) we don't have random access into the data to display
			// anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
			// it possible (and would be recommended if you want to search through tens of thousands of entries).
			ImGuiListClipper clipper;
			clipper.Begin(m_lineOffsets.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + m_lineOffsets[line_no];
					const char* line_end =
						(line_no + 1 < m_lineOffsets.Size) ? (buf + m_lineOffsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
	}

}