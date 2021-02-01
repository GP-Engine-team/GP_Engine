/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include <Windows.h>

namespace Engine::Core::HotReload
{
/**
 * @brief Corresponds to a dll
 */
class ReloadableCpp
{
private:
    std::string path;

    HMODULE module = nullptr;
    size_t lastRefreshTime = 0;
    std::map<std::string, void*> processes; // current successfully loaded processes

private:
    // Only call this when reloadableCpp.path is pointing towards a valid file.
    void load(const char* newFileSuffix = ".copy.dll")
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
        }
    }

    void unload()
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

public:
    /**
     * @brief Constructor of ReloadableCpp.
     * @param path The path of the dll, relative to the executable.
     *  Example : bin/myProgramDLL.dll
     */
    ReloadableCpp(const std::string& path) : path(path)
    {
    }

    ~ReloadableCpp()
    {
        unload();
    }

    /**
     * @brief Reload the code if the file has been modified since last load.
     * @return Returns true if the cpp has been reloaded.
     *  Returns false overwise.
     */
    bool Refresh()
    {
        // Open dll file
        HANDLE fileHandle = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (fileHandle == INVALID_HANDLE_VALUE)
            return false;

        // If the file has been modified since last load :
        uint64_t fileLastWriteTime;
        const bool hasFileBeenModified =
            (GetFileTime(fileHandle, NULL, NULL, (FILETIME*)&fileLastWriteTime) && lastRefreshTime < fileLastWriteTime);

        if (hasFileBeenModified)
        {
            unload();
            load();
            lastRefreshTime = fileLastWriteTime;
        }

        // Invalidate handle.
        CloseHandle(fileHandle);

        return hasFileBeenModified;
    }

    /**
     * @brief Add a process that will be loaded
     * @param processName The name of the process (e.g. the function name)
     */
    void addProcess(const std::string& processName)
    {
        processes.emplace(std::make_pair(processName, GetProcAddress((HMODULE)module, processName.c_str())));
    }

    /**
     * @brief Get the associated process.
     * @param processName The name of the process (e.g. the function name)
     * @return Returns a pointer to the process (e.g. the function)
     */
    template <class C_FUNCTION_TYPE>
    C_FUNCTION_TYPE* getProcess(const std::string& processName)
    {
        return (C_FUNCTION_TYPE*)processes[processName];
    }
};
} // namespace Engine::Core::HotReload