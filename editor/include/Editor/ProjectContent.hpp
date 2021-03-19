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
	uint64_t constexpr mix(char m, uint64_t s)
	{
		return ((s << 7) + ~(s >> 3)) + ~m;
	}

	uint64_t constexpr hash(const char* m)
	{
		return (*m) ? mix(*m, hash(m + 1)) : 0;
	}


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

		void renderMaterial(ImVec2& size)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			// -1 == uses default padding (style.FramePadding)
			ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
			ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);// UV coordinates for (32,32) in our texture
			ImVec4 tint_col = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background

			ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
		}

		void renderImage(ImVec2& size)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			// -1 == uses default padding (style.FramePadding)
			ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
			ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);// UV coordinates for (32,32) in our texture
			ImVec4 tint_col = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);     
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background

			ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
		}

		void renderModel(ImVec2& size)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			// -1 == uses default padding (style.FramePadding)
			ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
			ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);// UV coordinates for (32,32) in our texture
			ImVec4 tint_col = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);      
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background

			ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
		}

		void renderUnknowFormat(ImVec2& size)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			// -1 == uses default padding (style.FramePadding)
			ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
			ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);// UV coordinates for (32,32) in our texture
			ImVec4 tint_col = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);     
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background

			ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
		}

		void renderSound(ImVec2& size)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			// -1 == uses default padding (style.FramePadding)
			ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
			ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);// UV coordinates for (32,32) in our texture
			ImVec4 tint_col = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);    
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background

			ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
		}

		void renderShader(ImVec2& size)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			// -1 == uses default padding (style.FramePadding)
			ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
			ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);// UV coordinates for (32,32) in our texture
			ImVec4 tint_col = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background

			ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
		}

		void renderfolder(ImVec2& size, DirectoryInfo** pSelectectDir, DirectoryInfo& currentDir)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			// -1 == uses default padding (style.FramePadding)
			ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
			ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);// UV coordinates for (32,32) in our texture
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);       

			if (ImGui::ImageButton(my_tex_id, size, uv0, uv1, 2, bg_col, tint_col))
			{
				*pSelectectDir = &currentDir;
			}
		}

		void render()
		{
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			DirectoryInfo* pSelectedDirectory = pCurrentDirectory;

			if (pCurrentDirectory->pParent != nullptr)
			{
				if (ImGui::Button("Back"))
				{
					pSelectedDirectory = pCurrentDirectory->pParent;
				}
				ImGui::SameLine();
			}

			ImGui::TextUnformatted(pCurrentDirectory->path.string().c_str());

			ImVec2 size = ImVec2(32.0f, 32.0f);                     // Size of the image we want to make visible
			ImGuiStyle& style = ImGui::GetStyle();

			for (int i = 0; i < pCurrentDirectory->directories.size(); ++i)
			{
				ImGui::PushID(i);

				ImGui::BeginGroup();
				{
					renderfolder(size, &pSelectedDirectory, pCurrentDirectory->directories[i]);

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

					ImGui::TextUnformatted(pCurrentDirectory->directories[i].name.string().c_str());
				}
				ImGui::EndGroup();

				float last_button_x2 = ImGui::GetItemRectMax().x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x + size.x; // Expected position if next button was on same line
				if (i + 1 < pCurrentDirectory->directories.size() && next_button_x2 < window_visible_x2)
				{
					ImGui::SameLine();
				}

				ImGui::PopID();
			}

			for (int i = 0; i < pCurrentDirectory->files.size(); ++i)
			{
				ImGui::PushID(i);

				ImGui::BeginGroup();
				{
					switch (hash(pCurrentDirectory->files[i].extention.string().c_str())) //runtime
					{
					case hash(".obj"): //compile time
					case hash(".fbx"): //compile time
					case hash(".gltf"): //compile time
						renderModel(size);
						break;

					case hash(".mtl"): //compile time
						renderMaterial(size);
						break;

					case hash(".wav"): //compile time
					case hash(".mp3"): //compile time
						renderSound(size);
						break;

					case hash(".fs"): //compile time
					case hash(".vs"): //compile time
						renderShader(size);
						break;

					case hash(".jpg"): //compile time
					case hash(".png"): //compile time
					case hash(".jpeg"): //compile time
					case hash(".bmp"): //compile time
						renderImage(size);
						break;

					default:
						renderUnknowFormat(size);
						break;
					}

					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

						ImGui::Text("%lu bytes", pCurrentDirectory->files[i].size);
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}

					//Drag and drop
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("RESOURCE_PATH", (void*)&pCurrentDirectory->files[i].path, sizeof(&pCurrentDirectory->files[i].path));
						ImGui::TextUnformatted(pCurrentDirectory->files[i].filename.string().c_str());
						ImGui::EndDragDropSource();
					}

					ImGui::TextUnformatted(pCurrentDirectory->files[i].filename.string().c_str());
				}
				ImGui::EndGroup();

				float last_button_x2 = ImGui::GetItemRectMax().x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x + size.x; // Expected position if next button was on same line
				if (i + 1 < pCurrentDirectory->files.size() && next_button_x2 < window_visible_x2)
				{
					ImGui::SameLine();
				}

				ImGui::PopID();
			}

			pCurrentDirectory = pSelectedDirectory;
		}
	};

} // End of namespace Editor