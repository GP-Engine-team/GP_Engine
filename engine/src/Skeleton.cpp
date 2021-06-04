/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/Resources/Animation/Skeleton.hpp>
#include <Engine/Core/Debug/Assert.hpp>
#include <Engine/Resources/Importer/AssimpUtilities.hpp>
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

using namespace GPE;

void Skeleton::readHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
    GPE_ASSERT(src != nullptr, "Node should not be null.");

    dest.name           = src->mName.data;
    dest.transformation = GPE::toMat4(src->mTransformation);

    const unsigned int max = src->mNumChildren;
    for (unsigned int i = 0u; i < max; i++)
    {
        AssimpNodeData newData;
        readHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}