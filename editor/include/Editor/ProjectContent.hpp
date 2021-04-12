/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Debug/Log.hpp"
#include <Imgui/imgui.h>
#include <filesystem>
#include <memory>
#include <vector>

#include "Engine/Serialization/IInspectable.hpp"

#define RESOURCES_DIR "resources"

namespace Editor
{
struct FileInfo
{
    std::filesystem::path path;
    std::filesystem::path filename;
    std::filesystem::path extention;
    size_t                size = 0;
};

struct DirectoryInfo
{
    std::filesystem::path      name;
    DirectoryInfo*             pParent = nullptr;
    std::filesystem::path      path;
    std::vector<DirectoryInfo> directories = {};
    std::vector<FileInfo>      files       = {};

    bool containFile(std::filesystem::path name)
    {
        bool rst = false;
        for (int i = 0; i < files.size() && !rst; ++i)
        {
            rst |= files[i].filename == name;
        }
        return rst;
    }
};

class ProjectContent
{
protected:
    DirectoryInfo  resourcesTree;
    DirectoryInfo* pCurrentDirectory = nullptr;

    std::unique_ptr<GPE::IInspectable> importationSetting;

public:
    ProjectContent();

    void refreshResourcesList();

    void renderAndGetSelected(GPE::IInspectable*& selectedGameObject);
};

} // End of namespace Editor