/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <filesystem>
#include <vector>

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
};

class ProjectContent
{
protected:
    DirectoryInfo  resourcesTree;
    DirectoryInfo* pCurrentDirectory = nullptr;

public:
    ProjectContent();

    void refreshResourcesList();

    void render();
};

} // End of namespace Editor