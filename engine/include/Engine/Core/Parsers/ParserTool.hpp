/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

namespace Engine::Core::Parsers
{
inline bool isTitle(const std::string& line, std::string&& title)
{
    return line.substr(0, title.size()) == title;
}

template <typename T>
T parse(const std::string& line, size_t pos = 0, size_t len = std::string::npos)
{
    T i;
    std::istringstream(line.substr(pos, len)) >> i;
    return i;
}

inline void parseName(const std::string& line, std::string& name)
{
    std::size_t cursorStart = line.find_first_of(" ", 0) + 1;
    name = line.substr(cursorStart, line.size() - cursorStart); // Search first word between two space
    if (!isalpha(name.back()))
    {
        name.erase(name.end() - 1);
    }
}

/**
 * @brief parse the name of image integer in path
 *
 */
inline std::string parseNameInPath(const char* path)
{
    std::string src(path);

    std::size_t cursStart = src.find_last_of('/');
    if (cursStart == std::string::npos)
        cursStart = 0;
    cursStart += 1;
    std::size_t cursEnd = src.find_last_of('.');

    return src.substr(cursStart, cursEnd - cursStart);
}
} /*namespace Engine::Core::Parsers*/