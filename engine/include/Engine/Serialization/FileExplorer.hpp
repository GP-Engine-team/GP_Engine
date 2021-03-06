﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#ifdef _WIN64
#include <shobjidl.h>
#endif

#include <filesystem>

namespace GPE
{

#ifdef _WIN64

/**
 * @brief
 * @param title
 * @param filter :          COMDLG_FILTERSPEC rgSpec[] = {
                            {szJPG, L"*.jpg;*.jpeg"},
                            {szBMP, L"*.bmp"},
                            {szAll, L"*.*"}};
                            see :
 https://docs.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-ifiledialog-setfiletypes
 * @return
*/
std::filesystem::path openFileExplorerAndGetAbsoluePath(LPCWSTR                        title  = L"Open",
                                                        std::vector<COMDLG_FILTERSPEC> filter = {{L"All", L"*.*"}});

std::filesystem::path openFileExplorerAndGetRelativePath(LPCWSTR                        title  = L"Open",
                                                         std::vector<COMDLG_FILTERSPEC> filter = {{L"All", L"*.*"}});

std::filesystem::path openFolderExplorerAndGetAbsoluePath(LPCWSTR title = L"Open");

std::filesystem::path openFolderExplorerAndGetRelativePath(LPCWSTR title = L"Open");

void recycleFileOrDirectory(const std::filesystem::path& path);

#endif

} // namespace GPE
