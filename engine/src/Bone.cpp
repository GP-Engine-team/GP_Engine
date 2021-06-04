/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/Resources/Animation/Bone.hpp>

using namespace GPE;

/*reads keyframes from aiNodeAnim*/
Bone::Bone(const std::string& name, const aiNodeAnim* channel)
    : m_name(name), m_localTransform(GPM::Matrix4::identity()), m_id(0)
{
    unsigned int max = unsigned(channel->mNumPositionKeys);
    m_positions.reserve(max);

    for (unsigned int positionIndex = 0; positionIndex < max; ++positionIndex)
    {
        const aiVector3D  aiPosition = channel->mPositionKeys[positionIndex].mValue;
        const float       timeStamp  = float(channel->mPositionKeys[positionIndex].mTime);

        const KeyPosition data{GPE::toVec(aiPosition), timeStamp};

        m_positions.push_back(data);
    }

    max = channel->mNumRotationKeys;
    m_rotations.reserve(max);

    for (unsigned int rotationIndex = 0; rotationIndex < max; ++rotationIndex)
    {
        const aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        const float        timeStamp     = float(channel->mRotationKeys[rotationIndex].mTime);

        const KeyRotation data{GPE::toQuat(aiOrientation), timeStamp};

        m_rotations.push_back(data);
    }

    max = channel->mNumScalingKeys;
    m_scales.reserve(max);

    for (unsigned int keyIndex = 0; keyIndex < max; ++keyIndex)
    {
        const aiVector3D scale     = channel->mScalingKeys[keyIndex].mValue;
        const float      timeStamp = float(channel->mScalingKeys[keyIndex].mTime);

        const KeyScale data{GPE::toVec(scale), timeStamp};

        m_scales.push_back(data);
    }
}

/* Gets normalized value for Lerp & Slerp*/
float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor  = 0.0f;
    const float midWayLength = animationTime - lastTimeStamp;
    const float framesDiff   = nextTimeStamp - lastTimeStamp;
    scaleFactor        = midWayLength / framesDiff;
    return scaleFactor;
}

/* figures out which position keys to interpolate b/w and performs the interpolation
and returns the translation matrix */
GPM::Mat4 Bone::interpolatePosition(float animationTime)
{
    if (m_positions.size() == 1u)
        return GPM::Transform::translation(m_positions[0].position);

    const int       p0Index     = getPositionIndex(animationTime);
    const int       p1Index     = p0Index + 1;
    const float     scaleFactor = getScaleFactor(m_positions[p0Index].timeStamp, m_positions[p1Index].timeStamp, animationTime);
    const GPM::Vec3 finalPosition = m_positions[p0Index].position.lerp(m_positions[p1Index].position, scaleFactor);
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

    const int   p0Index     = getRotationIndex(animationTime);
    const int   p1Index     = p0Index + 1;
    const float scaleFactor = getScaleFactor(m_rotations[p0Index].timeStamp, m_rotations[p1Index].timeStamp, animationTime);
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

    const int       p0Index     = getScaleIndex(animationTime);
    const int       p1Index     = p0Index + 1;
    const float     scaleFactor = getScaleFactor(m_scales[p0Index].timeStamp, m_scales[p1Index].timeStamp, animationTime);
    const GPM::Vec3 finalScale  = m_scales[p0Index].scale.lerp(m_scales[p1Index].scale, scaleFactor);
    return GPM::Transform::scaling(finalScale);
}

/* Interpolates b/w positions,rotations & scaling keys based on the curren time of the
animation and prepares the local transformation matrix by combining all keys tranformations */
void Bone::update(float animationTime)
{
    const GPM::Mat4 translation = interpolatePosition(animationTime);
    const GPM::Mat4 rotation    = interpolateRotation(animationTime);
    const GPM::Mat4 scale       = interpolateScaling(animationTime);
    m_localTransform            = translation * rotation * scale;
}

/* Gets the current index on mKeyPositions to interpolate to based on the current
animation time */
int Bone::getPositionIndex(float animationTime)
{
    const size_t max = m_positions.size() - 1u;
    for (size_t index = 0u; index < max; ++index)
    {
        if (animationTime < m_positions[index + 1ul].timeStamp)
            return int(index);
    }

    assert(0);
    return -1;
}

/* Gets the current index on mKeyRotations to interpolate to based on the current
animation time */
int Bone::getRotationIndex(float animationTime)
{
    const size_t max = m_rotations.size() - 1ul;
    for (size_t index = 0u; index < max; ++index)
    {
        if (animationTime < m_rotations[index + 1ul].timeStamp)
            return int(index);
    }

    assert(0);
    return -1;
}

/* Gets the current index on mKeyScalings to interpolate to based on the current
animation time */
int Bone::getScaleIndex(float animationTime)
{
    const size_t max = m_scales.size() - 1ul;
    for (size_t index = 0u; index < max; ++index)
    {
        if (animationTime < m_scales[index + 1ul].timeStamp)
            return int(index);
    }

    assert(0);
    return -1;
}

template <>
void GPE::save(BinarySaver& context, const Bone& saved, const BinarySaver::SaveInfo* info)
{
    saved.save(context);
}

template <>
void GPE::load(BinaryLoader& context, Bone& loaded, const BinaryLoader::LoadInfo* info)
{
    loaded.load(context);
}

void GPE::Bone::save(BinarySaver& saver) const
{
    GPE::save(saver, m_positions, nullptr);
    GPE::save(saver, m_rotations, nullptr);
    GPE::save(saver, m_scales, nullptr);
    GPE::save(saver, m_localTransform, nullptr);
    GPE::save(saver, m_name, nullptr);
}
void GPE::Bone::load(BinaryLoader& loader)
{
    GPE::load(loader, m_positions, nullptr);
    GPE::load(loader, m_rotations, nullptr);
    GPE::load(loader, m_scales, nullptr);
    GPE::load(loader, m_localTransform, nullptr);
    GPE::load(loader, m_name, nullptr);
}