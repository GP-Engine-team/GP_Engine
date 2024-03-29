﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/Texture.hpp>

#include <filesystem>
#include <list>
#include <memory>

namespace GPE
{
class GameObject;
class IInspectable;
} // namespace GPE

#define RESOURCES_DIR "resources"

namespace Editor
{
struct FileInfo
{
    std::filesystem::path path;
    std::filesystem::path filename;
    std::filesystem::path extention;
    size_t                size           = 0u;
    bool                  isInRenameMode = false;
};

struct DirectoryInfo
{
    std::filesystem::path    name;
    DirectoryInfo*           pParent = nullptr;
    std::filesystem::path    path;
    std::list<DirectoryInfo> directories    = {};
    std::list<FileInfo>      files          = {};
    bool                     isInRenameMode = false;

    bool containDirectory(std::filesystem::path name)
    {
        bool rst = false;

        for (auto&& it = directories.cbegin(); it != directories.cend() && !rst; ++it)
        {
            rst |= it->name.filename() == name;
        }
        return rst;
    }

    bool containFile(std::filesystem::path name)
    {
        bool rst = false;

        for (auto&& it = files.cbegin(); it != files.cend() && !rst; ++it)
        {
            rst |= it->filename == name;
        }
        return rst;
    }

    size_t computeSize() const
    {
        size_t rst = 0;

        for (auto&& it = files.cbegin(); it != files.cend(); ++it)
        {
            rst += it->size;
        }

        for (auto&& it = directories.cbegin(); it != directories.cend(); ++it)
        {
            rst += it->computeSize();
        }
        return rst;
    }
};

class ProjectContent
{
protected:
    // TODO : icone -> icon
    GPE::Texture m_folderIcone;
    GPE::Texture m_textureIcone;
    GPE::Texture m_materialIcone;
    GPE::Texture m_sceneIcone;
    GPE::Texture m_meshIcone;
    GPE::Texture m_shaderIcone;
    GPE::Texture m_soundIcone;
    GPE::Texture m_unknowIcone;
    GPE::Texture m_prefabIcone;
    GPE::Texture m_skeletonIcon;
    GPE::Texture m_animationIcon;
    GPE::Texture m_skinIcon;

    DirectoryInfo  resourcesTree;
    DirectoryInfo* pCurrentDirectory = nullptr;

    std::unique_ptr<GPE::IInspectable> importationSetting = nullptr;

    class Editor* m_editorContext = nullptr;

protected:
    void tryToSetCurrentCirToPreviousLocation(const std::filesystem::path& previousPath);

public:
    ProjectContent(Editor& editorContext);

    void refreshResourcesList();

    void renderAndGetSelected(GPE::IInspectable*& selectedGameObject);

    void createNewScene();
    void removeFile(const std::filesystem::path& path);
};

} // End of namespace Editor