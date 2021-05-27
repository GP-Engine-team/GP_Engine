/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/Resources/Animation/Bone.hpp>

using namespace GPE;

/*reads keyframes from aiNodeAnim*/
Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
    : m_name(name), m_id(ID), m_localTransform(GPM::Matrix4::identity())
{
    m_positions.reserve(channel->mNumPositionKeys);
    for (int positionIndex = 0; positionIndex < channel->mNumPositionKeys; ++positionIndex)
    {
        aiVector3D  aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float       timeStamp  = channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position  = GPE::toVec(aiPosition);
        data.timeStamp = timeStamp;
        m_positions.push_back(data);
    }

    m_rotations.reserve(channel->mNumPositionKeys);
    for (int rotationIndex = 0; rotationIndex < channel->mNumRotationKeys; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float        timeStamp     = channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation  data;
        data.orientation = GPE::toQuat(aiOrientation);
        data.timeStamp   = timeStamp;
        m_rotations.push_back(data);
    }

    m_scales.reserve(channel->mNumScalingKeys);
    for (int keyIndex = 0; keyIndex < channel->mNumScalingKeys; ++keyIndex)
    {
        aiVector3D scale     = channel->mScalingKeys[keyIndex].mValue;
        float      timeStamp = channel->mScalingKeys[keyIndex].mTime;
        KeyScale   data;
        data.scale     = GPE::toVec(scale);
        data.timeStamp = timeStamp;
        m_scales.push_back(data);
    }
}

/* Gets normalized value for Lerp & Slerp*/
float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor  = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff   = nextTimeStamp - lastTimeStamp;
    scaleFactor        = midWayLength / framesDiff;
    return scaleFactor;
}

/* figures out which position keys to interpolate b/w and performs the interpolation
and returns the translation matrix */
GPM::Mat4 Bone::interpolatePosition(float animationTime)
{
    if (m_positions.size() == 1)
        return GPM::Transform::translation(m_positions[0].position);

    int   p0Index     = getPositionIndex(animationTime);
    int   p1Index     = p0Index + 1;
    float scaleFactor = getScaleFactor(m_positions[p0Index].timeStamp, m_positions[p1Index].timeStamp, animationTime);
    GPM::Vec3 finalPosition = GPM::lerp(m_positions[p0Index].position, m_positions[p1Index].position, scaleFactor);
    return GPM::Transform::translation(finalPosition);
}

/* figures out which rotations keys to interpolate b/w and performs the interpolation
and returns the rotation matrix */
GPM::Mat4 Bone::interpolateRotation(float animationTime)
{
    if (m_rotations.size() == 1)
    {
        auto rotation = m_rotations[0].orientation.normalized();
        return GPM::toMatrix4(rotation);
    }

    int   p0Index     = getRotationIndex(animationTime);
    int   p1Index     = p0Index + 1;
    float scaleFactor = getScaleFactor(m_rotations[p0Index].timeStamp, m_rotations[p1Index].timeStamp, animationTime);
    GPM::Quat finalRotation = m_rotations[p0Index].orientation.nlerp(m_rotations[p1Index].orientation, scaleFactor);
    finalRotation           = finalRotation.normalized();
    return GPM::toMatrix4(finalRotation);
}

/* figures out which scaling keys to interpolate b/w and performs the interpolation
and returns the scale matrix */
GPM::Mat4 Bone::interpolateScaling(float animationTime)
{
    if (m_scales.size() == 1)
        return GPM::Transform::scaling(m_scales[0].scale);

    int       p0Index     = getScaleIndex(animationTime);
    int       p1Index     = p0Index + 1;
    float     scaleFactor = getScaleFactor(m_scales[p0Index].timeStamp, m_scales[p1Index].timeStamp, animationTime);
    GPM::Vec3 finalScale  = GPM::lerp(m_scales[p0Index].scale, m_scales[p1Index].scale, scaleFactor);
    return GPM::Transform::scaling(finalScale);
}

/* Interpolates b/w positions,rotations & scaling keys based on the curren time of the
animation and prepares the local transformation matrix by combining all keys tranformations */
void Bone::update(float animationTime)
{
    GPM::Mat4 translation = interpolatePosition(animationTime);
    GPM::Mat4 rotation    = interpolateRotation(animationTime);
    GPM::Mat4 scale       = interpolateScaling(animationTime);
    m_localTransform      = translation * rotation * scale;
}

/* Gets the current index on mKeyPositions to interpolate to based on the current
animation time */
int Bone::getPositionIndex(float animationTime)
{
    for (int index = 0; index < m_positions.size() - 1; ++index)
    {
        if (animationTime < m_positions[index + 1].timeStamp)
            return index;
    }
    assert(0);
}

/* Gets the current index on mKeyRotations to interpolate to based on the current
animation time */
int Bone::getRotationIndex(float animationTime)
{
    for (int index = 0; index < m_rotations.size() - 1; ++index)
    {
        if (animationTime < m_rotations[index + 1].timeStamp)
            return index;
    }
    assert(0);
}

/* Gets the current index on mKeyScalings to interpolate to based on the current
animation time */
int Bone::getScaleIndex(float animationTime)
{
    for (int index = 0; index < m_scales.size() - 1; ++index)
    {
        if (animationTime < m_scales[index + 1].timeStamp)
            return index;
    }
    assert(0);
}