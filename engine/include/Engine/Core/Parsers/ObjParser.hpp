/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/Model.hpp"

namespace GPE
{

std::vector<SubModel> importeSingleModel(const char* assetPath, ResourceManagerType& resourceManager) noexcept;

} // namespace GPE