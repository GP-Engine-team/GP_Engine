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

#define ADD_PROCESS(rcpp, function) rcpp.addProcess(#function);
#define GET_PROCESS(rcpp, function) rcpp.getProcess<decltype(function)>(#function);

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
    void load(const char *newFileSuffix = ".copy.dll");

    void unload();

public:
    /**
     * @brief Constructor of ReloadableCpp.
     * @param path The path of the dll, relative to the executable.
     *  Example : bin/myProgramDLL.dll
     */
    ReloadableCpp(const std::string &path);
    ~ReloadableCpp();

    ReloadableCpp(const ReloadableCpp &) = delete;
    ReloadableCpp(ReloadableCpp &&) = delete;
    ReloadableCpp &operator=(ReloadableCpp &&) = delete;
    ReloadableCpp &operator=(const ReloadableCpp &) = delete;

    /**
     * @brief Reload the code if the file has been modified since last load.
     * @return Returns true if the cpp has been reloaded.
     *  Returns false overwise.
     */
    bool refresh();

    /**
     * @brief Add a process that will be loaded
     * @param processName The name of the process (e.g. the function name)
     */
    void addProcess(const std::string &processName);

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