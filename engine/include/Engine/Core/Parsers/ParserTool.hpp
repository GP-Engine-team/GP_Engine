/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

namespace GPE
{
inline bool stringStartsWith(const std::string& src, std::string&& title) noexcept;

template <typename T>
T parse(const std::string& srtc, size_t pos = 0, size_t len = std::string::npos) noexcept;


inline void removeUntilFirstSpace(const std::string& src, std::string& dest) noexcept;

/**
 * @brief parse the name of image integer in path
 *
 */
inline std::string removeUntilFirstSpaceInPath(const char* path) noexcept;

#include "ParserTool.inl"

} /*namespace GPE*/