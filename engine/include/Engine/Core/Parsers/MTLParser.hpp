/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>
#include <vector>

#include "Engine/Resources/Type.hpp"

#define PATH_MTL "./resources/materials/"
#define EXTENSION_MTL ".mtl"

namespace GPE
{
/**
 * @brief Parse file .mtl and return buffer of data parsing
 *
 * @param materialPath
 * @param materials
 */
void loadMTL(std::string& materialPath, std::vector<MaterialAttrib>& materials) noexcept;

} // namespace GPE