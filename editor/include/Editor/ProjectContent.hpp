/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/System/SystemsManager.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include <Imgui/imgui.h>
#include <vector>
#include <filesystem>
#include <string>

#define RESOURCES_DIR "resources"

namespace Editor
{

	struct FileInfo
	{
		std::filesystem::path path;
		std::filesystem::path filename;
		std::filesystem::path extention;
		size_t size = 0;
	};

	struct DirectoryInfo
	{
		std::filesystem::path name;
		DirectoryInfo* pParent = nullptr;
		std::filesystem::path path;
		std::vector<DirectoryInfo> directories = {};
		std::vector<FileInfo> files = {};

	};

	class ProjectContent
	{
	protected:

		DirectoryInfo resourcesTree;
		DirectoryInfo* pCurrentDirectory = nullptr;

	public:

		ProjectContent()
		{
			resourcesTree.name = RESOURCES_DIR;
			resourcesTree.path = std::filesystem::current_path() / RESOURCES_DIR;

			refreshResourcesList();
			pCurrentDirectory = &resourcesTree;
		}

		void refreshResourcesList()
		{
			resourcesTree.files.clear();
			resourcesTree.directories.clear();

			std::filesystem::path path = std::filesystem::current_path() / RESOURCES_DIR;

			DirectoryInfo* pCurrentNode = &resourcesTree;

			for (std::filesystem::recursive_directory_iterator next(path), end; next != end; ++next)
			{
				if (next->is_directory())
				{
					if (pCurrentNode->pParent != nullptr)
					{
						while (next->path().parent_path() != pCurrentNode->path)
						{
							pCurrentNode = pCurrentNode->pParent;
						}
					}

					pCurrentNode = &pCurrentNode->directories.emplace_back(DirectoryInfo{ next->path().stem(), pCurrentNode, next->path() });
				}
				else if (next->is_regular_file())
				{
					if (pCurrentNode->pParent != nullptr)
					{
						while (next->path().parent_path() != pCurrentNode->path)
						{
							pCurrentNode = pCurrentNode->pParent;
						}
					}
					pCurrentNode->files.emplace_back(FileInfo{ next->path(), next->path().filename(), next->path().extension(), next->file_size() });
				}
			}

			explore(resourcesTree);
		}

		std::string doTab(int num)
		{
			std::string str;

			for (size_t i = 0; i < num; ++i)
			{
				str += '\t';
			}
			return str;
		}

		void explore(DirectoryInfo dir, int tab = 0)
		{
			std::cout << doTab(tab) + dir.name.string() + " - Directories : " + std::to_string(dir.directories.size()) + " - Files : " + std::to_string(dir.files.size()) << std::endl;;

			for (auto&& file : dir.files)
			{
				std::cout << doTab(tab + 1) + file.filename.string() + " - " + file.extention.string() + " - " + std::to_string(file.size) + "bytes " << std::endl;
			}

			for (auto&& direct : dir.directories)
			{
				explore(direct, tab + 1);
			}
		}

		void render()
		{
			ImGuiIO& io = ImGui::GetIO();
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

			for (int i = 0; i < pCurrentDirectory->directories.size(); ++i)
			{
				ImVec2 size = ImVec2(32.0f, 32.0f);                     // Size of the image we want to make visible
				ImGuiStyle& style = ImGui::GetStyle();

				ImGui::PushID(i);
				if (ImGui::BeginTable((std::to_string(i) + "table").c_str(), 1))
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);                            // -1 == uses default padding (style.FramePadding)
					ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
					ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);// UV coordinates for (32,32) in our texture
					ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
					ImVec4 tint_col = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);       // No tint

					if (ImGui::ImageButton(my_tex_id, size, uv0, uv1, 2, bg_col, tint_col));

					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

						size_t dirSize = 0;
						for (auto&& file : pCurrentDirectory->directories[i].files)
							dirSize += file.size;

						ImGui::Text("%lu bytes", dirSize);
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					ImGui::TextUnformatted(pCurrentDirectory->directories[i].name.string().c_str());

					ImGui::EndTable();

				}

				float last_button_x2 = size.x /*ImGui::GetItemRectMax().x*/;
				std::cout << last_button_x2 << std::endl;
				float next_button_x2 =
					last_button_x2 + style.ItemSpacing.x + size.x; // Expected position if next button was on same line
				if (i + 1 < pCurrentDirectory->directories.size() && next_button_x2 < window_visible_x2)
				{
					std::cout << "OK" << std::endl;
					ImGui::SameLine();
				}
				ImGui::PopID();
			}
		}
	};

} // End of namespace Editor
