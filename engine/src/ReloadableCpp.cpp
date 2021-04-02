#include "Engine/Core/HotReload/ReloadableCpp.hpp"
#include "Engine/Core/Debug/Log.hpp"

#include <Windows.h>

using namespace GPE;

bool ReloadableCpp::load(const char* newFileSuffix)
{
    std::string copyFilename = path + newFileSuffix;

    // Create a new file and copy the content of reloadableCpp into the new file to prevent data race.
    // If the file already exist, then overwrite the old file.
    if (CopyFile(path.c_str(), copyFilename.c_str(), FALSE))
    {
        // Load copied file.
        // If loaded successfully :
        if (module = LoadLibrary(copyFilename.c_str()))
        {
            for (auto& p : processes)
            {
                p.second = GetProcAddress((HMODULE)module, p.first.c_str());
            }
        }
        else
        {

            Log::getInstance()->logError(
                "dll not loaded properly : Check if the correct path has been set and if other dll dependencies "
                "are in the same place.");
            return false;
        }
    }
    else
    {
        Log::getInstance()->logError("Couldn't create " + copyFilename + " or copy " + path + " into it.");
        return false;
    }

    return true;
}

void ReloadableCpp::unload()
{
    // If dll module valid
    if (module)
    {
        // Free the dll module
        FreeLibrary(module);
        module = nullptr;
        for (auto p : processes)
        {
            p.second = nullptr;
        }
    }
}

ReloadableCpp::ReloadableCpp(const std::string& path) : path(path)
{
}

ReloadableCpp::~ReloadableCpp()
{
    unload();
}

bool ReloadableCpp::refresh()
{
    // Open dll file
    HANDLE fileHandle = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL, NULL);

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        // Couldn't open file. It might be because the file is currently used.
        return false;
    }

    // If the file has been modified since last load :
    uint64_t   fileLastWriteTime;
    const bool hasFileBeenModified =
        (GetFileTime(fileHandle, NULL, NULL, (FILETIME*)&fileLastWriteTime) && lastRefreshTime < fileLastWriteTime);

    if (hasFileBeenModified)
    {
        if (onUnload)
        {
            onUnload();
        }
        unload();
        if (!load())
        {
            return false;
        }
        lastRefreshTime = fileLastWriteTime;
    }

    // Invalidate handle.
    CloseHandle(fileHandle);

    return hasFileBeenModified;
}

void ReloadableCpp::addProcess(const std::string& processName)
{
    processes.emplace(std::make_pair(processName, GetProcAddress((HMODULE)module, processName.c_str())));
}