/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/Model.hpp>
#include <Engine/Resources/Animation/Bone.hpp>
#include <Engine/Resources/Importer/AssimpUtilities.hpp>
#include <Engine/Resources/Mesh.hpp>
#include <GPM/Matrix4.hpp>
#include <map>
#include <string>
#include <vector>

namespace GPE
{
class Animation
{
private:
    float                                      m_duration;
    int                                        m_ticksPerSecond;
    std::vector<Bone>                          m_bones;

public:
    void readMissingBones(const aiAnimation* animation, class Skeleton& skeleton);

public:
    Animation() = default;

    Animation(const aiAnimation* aiAnim, Skeleton& skeleton);
    Bone* findBone(const std::string& name);

public:
    GETTER_BY_VALUE(TicksPerSecond, m_ticksPerSecond);
    GETTER_BY_VALUE(Duration, m_duration);
};

} // namespace GPE