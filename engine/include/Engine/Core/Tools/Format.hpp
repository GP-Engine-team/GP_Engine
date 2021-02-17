/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

namespace GPE
{
/**
 * @brief std::string formatting like sprintf. Use std::format if you use __cplusplus > 201703L
 *
 * @link https://stackoverflow.com/a/26221725/12703286
 * @tparam Args
 * @param format
 * @param args
 * @return std::string
 */
template <typename... Args>
[[nodiscard]] std::string stringFormat(const std::string& format, Args... args) noexcept;

#include "Format.inl"

} /*namespace GPE*/

