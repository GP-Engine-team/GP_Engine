/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Tools/ClassUtility.hpp"
#include <GPM/Matrix4.hpp>
#include <memory>
#include <string>

namespace GPE
{
class Skeleton
{
private:
    std::string m_name;

    size_t                         m_nbBones = 0;
    std::unique_ptr<GPM::Mat4[]>   m_relativeBones;
    std::unique_ptr<GPM::Mat4[]>   m_inversedWorldBones;
    std::unique_ptr<int[]>      m_parentBoneIndices;
    std::unique_ptr<std::string[]> m_boneNames;

public:
    Skeleton()                      = default;
    Skeleton(const Skeleton& other) = default;
    Skeleton(Skeleton&& other)      = default;
    ~Skeleton()                     = default;
    Skeleton& operator=(Skeleton const& other) = default;
    Skeleton& operator=(Skeleton&& other) = default;

    void PrintHierarchy() const;

    GETTER_BY_VALUE(NbBones, m_nbBones);
    GETTER_BY_VALUE(Name, m_name);
};

} /*namespace GPE*/
