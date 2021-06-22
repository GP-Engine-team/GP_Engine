/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <filesystem>

namespace GPE
{
/**
 * @brief This class is usefull for the inspection of path
 * @tparam T
 */
template <typename T>
struct PathTo
{
    std::string path;

    void operator=(const std::string& newPath)
    {
        path = newPath;
    }

    void operator=(std::string&& newPath)
    {
        path = std::move(newPath);
    }

    operator std::string&()
    {
        return path;
    }

    operator const std::string&() const
    {
        return path;
    }
};

} // namespace GPE