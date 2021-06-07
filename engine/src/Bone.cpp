/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/Resources/Animation/Bone.hpp>
#include <algorithm>

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

/* Gets normalized value for Lerp & Nlerp*/
float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) const
{
    float scaleFactor  = 0.0f;
    const float midWayLength = animationTime - lastTimeStamp;
    const float framesDiff   = nextTimeStamp - lastTimeStamp;
    scaleFactor        = midWayLength / framesDiff;
    return scaleFactor;
}


GPM::Vec3 Bone::getInterpolatedPosition(float time) const
{
    if (m_positions.size() == 1u)
        return m_positions[0].position;

    const int   p0Index = getPositionIndex(time);
    const int   p1Index = p0Index + 1;
    const float scaleFactor = getScaleFactor(m_positions[p0Index].timeStamp, m_positions[p1Index].timeStamp, time);
    return m_positions[p0Index].position.lerp(m_positions[p1Index].position, scaleFactor);
}
GPM::Quat Bone::getInterpolatedRotation(float time) const
{
    if (m_rotations.size() == 1)
    {
        return m_rotations[0].orientation.normalized();
    }

    const int   p0Index = getRotationIndex(time);
    const int   p1Index = p0Index + 1;
    const float scaleFactor = getScaleFactor(m_rotations[p0Index].timeStamp, m_rotations[p1Index].timeStamp, time);
    GPM::Quat finalRotation = m_rotations[p0Index].orientation.nlerp(m_rotations[p1Index].orientation, scaleFactor);
    return finalRotation.normalized();
}
GPM::Vec3 Bone::getInterpolatedScale(float time) const
{
    if (m_scales.size() == 1)
        return m_scales[0].scale;

    const int   p0Index     = getScaleIndex(time);
    const int   p1Index     = p0Index + 1;
    const float scaleFactor = getScaleFactor(m_scales[p0Index].timeStamp, m_scales[p1Index].timeStamp, time);
    return m_scales[p0Index].scale.lerp(m_scales[p1Index].scale, scaleFactor);
}

/* figures out which position keys to interpolate b/w and performs the interpolation
and returns the translation matrix */
GPM::Mat4 Bone::interpolatePosition(float animationTime)
{
    return GPM::Transform::translation(getInterpolatedPosition(animationTime));
}

/* figures out which rotations keys to interpolate b/w and performs the interpolation
and returns the rotation matrix */
GPM::Mat4 Bone::interpolateRotation(float animationTime)
{
    return GPM::toMatrix4(getInterpolatedRotation(animationTime));
}

/* figures out which scaling keys to interpolate b/w and performs the interpolation
and returns the scale matrix */
GPM::Mat4 Bone::interpolateScaling(float animationTime)
{
    return GPM::Transform::scaling(getInterpolatedScale(animationTime));
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
int Bone::getPositionIndex(float animationTime) const
{
    auto it = std::upper_bound(m_positions.begin() + 1, m_positions.end(), animationTime,
                         [&](const float& animTime, const GPE::KeyPosition& pos) { return animTime < pos.timeStamp; });

    if (it != m_positions.end())
        return &(*it) - m_positions.data() - 1;

    assert(false);
    return -1;
}

/* Gets the current index on mKeyRotations to interpolate to based on the current
animation time */
int Bone::getRotationIndex(float animationTime) const
{
    auto it = std::upper_bound(m_rotations.begin() + 1, m_rotations.end(), animationTime,
                         [&](const float& animTime, const GPE::KeyRotation& rot) { return animTime < rot.timeStamp; });

    if (it != m_rotations.end())
        return &(*it) - m_rotations.data() - 1;

    assert(false);
    return -1;
}

/* Gets the current index on mKeyScalings to interpolate to based on the current
animation time */
int Bone::getScaleIndex(float animationTime) const
{
    auto it = std::upper_bound(m_scales.begin() + 1, m_scales.end(), animationTime,
                         [&](const float& animTime, const GPE::KeyScale& scale) { return animTime < scale.timeStamp; });

    if (it != m_scales.end())
        return &(*it) - m_scales.data() - 1;

    assert(false);
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