/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Model.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Prefab.hpp"
#include "Engine/Resources/RenderBuffer.hpp"
#include "Engine/Resources/RenderTexture.hpp"
#include "Engine/Resources/ResourcesManager.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Sound.hpp"
#include "Engine/Resources/Texture.hpp"
#include "Engine/Serialization/SavedScene.hpp"
#include "Engine/Resources/Animation/Animation.hpp"
#include "Engine/Resources/Animation/Skeleton.hpp"
#include "Engine/Resources/Animation/Skin.hpp"

#include <GPM/Shape3D/AABB.hpp>
#include <GPM/Shape3D/Sphere.hpp>

#include <vector>

namespace GPE
{

using ResourceManagerType =
    ResourcesManager<std::vector<Mesh>, Model::CreateArg, Mesh, Shader, Texture, RenderBuffer, RenderTexture,
                     std::vector<Material>, Material, Sound::Buffer, SavedScene, GPM::Sphere, GPM::AABB, SharedPrefab>;

using AnimResourceManagerType = ResourcesManager<Animation, Skeleton, Skin>;
} // namespace GPE
