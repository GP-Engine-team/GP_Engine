#include "Engine/Serialization/FileExplorer.hpp"

#include "Engine/Core/Debug/Log.hpp"

namespace GPE
{

#ifdef _WIN64

std::filesystem::path openFileExplorerAndGetAbsoluePath(LPCWSTR title, std::vector<COMDLG_FILTERSPEC> filter)
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

            if (FAILED(pFileOpen->SetFileTypes(static_cast<UINT>(filter.size()), filter.data())))
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

std::filesystem::path openFileExplorerAndGetRelativePath(LPCWSTR title, std::vector<COMDLG_FILTERSPEC> filter)
{
    return std::filesystem::relative(openFileExplorerAndGetAbsoluePath(title, filter));
}

#endif

} // namespace GPE