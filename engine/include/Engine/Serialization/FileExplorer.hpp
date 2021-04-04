/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#ifdef _WIN64
#include <shobjidl.h>
#endif

#include "Engine/Core/Debug/Log.hpp"
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
std::filesystem::path openFileExplorer(LPCWSTR                        title  = L"Open",
                                       std::vector<COMDLG_FILTERSPEC> filter = {{L"All", L"*.*"}})
{
    std::filesystem::path src;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen = nullptr;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog,
                              reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            if (FAILED(pFileOpen->SetTitle(title)))
            {
                FUNCT_ERROR("Invalid title to init windows file explorer");
                return src;
            }

            if (FAILED(pFileOpen->SetFileTypes(filter.size(), filter.data())))
            {
                FUNCT_ERROR("Invalid filter to init windows file explorer");
                return src;
            }

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    LPWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        src = pszFilePath;
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return src;
}
#endif

} // namespace GPE
