/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Model.hpp"
#include "Engine/Resources/RenderBuffer.hpp"
#include "Engine/Resources/RenderTexture.hpp"
#include "Engine/Resources/ResourcesManager.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Sound.hpp"
#include "Engine/Resources/Texture.hpp"

#include <vector>

namespace GPE
{
using ResourceManagerType = ResourcesManager<std::vector<Mesh>, Model::CreateArg, Mesh, Shader, Texture, RenderBuffer,
                                             RenderTexture, std::vector<Material>, Material, Sound::Buffer>;

} // namespace GPE
