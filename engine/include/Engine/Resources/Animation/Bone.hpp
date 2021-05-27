/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once 

#include <GPM/Vector3.hpp>
#include <GPM/Quaternion.hpp>
#include <GPM/Matrix4.hpp>
#include <GPM/Transform.hpp>
#include <GPM/Calc.hpp>
#include <GPM/conversion.hpp>
#include <vector>
#include <string>
#include <cassert>
#include <Engine/Resources/Importer/AssimpUtilities.hpp>
#include <Engine/Core/Tools/ClassUtility.hpp>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

namespace GPE
{

struct KeyPosition
{
    GPM::Vec3 position;
    float     timeStamp;
};

struct KeyRotation
{
    GPM::Quat orientation;
    float     timeStamp;
};

struct KeyScale
{
    GPM::Vec3 scale;
    float     timeStamp;
};

class Bone
{
private:
    std::vector<KeyPosition> m_positions;
    std::vector<KeyRotation> m_rotations;
    std::vector<KeyScale>    m_scales;

    /**
     * @brief The new local transform (relative to the parent bone) computed after update(), with the transform variations.
    */
    GPM::Mat4   m_localTransform;
    std::string m_name;
    int         m_id;

private:
    /* Gets normalized value for Lerp & Slerp*/
    float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

    /* figures out which position keys to interpolate b/w and performs the interpolation
    and returns the translation matrix */
    GPM::Mat4 interpolatePosition(float animationTime);

    /* figures out which rotations keys to interpolate b/w and performs the interpolation
    and returns the rotation matrix */
    GPM::Mat4 interpolateRotation(float animationTime);

    /* figures out which scaling keys to interpolate b/w and performs the interpolation
    and returns the scale matrix */
    GPM::Mat4 interpolateScaling(float animationTime);

public:
    /*reads keyframes from aiNodeAnim*/
    Bone(const std::string& name, int ID, const aiNodeAnim* channel);

    /* Interpolates b/w positions,rotations & scaling keys based on the curren time of the
    animation and prepares the local transformation matrix by combining all keys tranformations */
    void update(float animationTime);

    /* Gets the current index on mKeyPositions to interpolate to based on the current
    animation time */
    int getPositionIndex(float animationTime);

    /* Gets the current index on mKeyRotations to interpolate to based on the current
    animation time */
    int getRotationIndex(float animationTime);

    /* Gets the current index on mKeyScalings to interpolate to based on the current
    animation time */
    int getScaleIndex(float animationTime);

    GETTER_BY_CONST_REF(LocalTransform, m_localTransform);
    GETTER_BY_CONST_REF(Name, m_name);
    GETTER_BY_CONST_REF(ID, m_id);
};

} // namespace