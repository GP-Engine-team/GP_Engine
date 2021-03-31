﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Model.hpp"
#include "Engine/Resources/Mesh.hpp"

namespace GPE
{

void importeModel(const char* assetPath) noexcept;

void createTextureFile();
void createMaterialFile();
void createMeshFile(const char* outPath, const Mesh::CreateIndiceBufferArg& arg);

} // namespace GPE