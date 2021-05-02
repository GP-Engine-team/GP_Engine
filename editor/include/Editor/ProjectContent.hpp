/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <filesystem>
#include <list>
#include <memory>

namespace GPE
{
class IInspectable;
}

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
    std::filesystem::path    name;
    DirectoryInfo*           pParent = nullptr;
    std::filesystem::path    path;
    std::list<DirectoryInfo> directories = {};
    std::list<FileInfo>      files       = {};

    bool containFile(std::filesystem::path name)
    {
        bool rst = false;

        for (auto&& it = files.cbegin(); it != files.cend() && !rst; ++it)
        {
            rst |= it->filename == name;
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