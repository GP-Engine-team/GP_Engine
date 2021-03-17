/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Model.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"

namespace GPE
{

Model::CreateArg importeSingleModel(const char* assetPath, ResourceManagerType& resourceManager,
                                    Mesh::EBoundingVolume boundingVolumeType = Mesh::EBoundingVolume::SPHERE) noexcept;

} // namespace GPE
