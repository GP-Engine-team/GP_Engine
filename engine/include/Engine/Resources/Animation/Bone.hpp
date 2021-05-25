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
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale>    m_Scales;
    int                      m_NumPositions;
    int                      m_NumRotations;
    int                      m_NumScalings;

    GPM::Mat4   m_LocalTransform;
    std::string m_Name;
    int         m_ID;

public:
    /*reads keyframes from aiNodeAnim*/
    Bone(const std::string& name, int ID, const aiNodeAnim* channel)
        : m_Name(name), m_ID(ID), m_LocalTransform(GPM::Matrix4::identity())
    {
        m_NumPositions = channel->mNumPositionKeys;

        for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
        {
            aiVector3D  aiPosition = channel->mPositionKeys[positionIndex].mValue;
            float       timeStamp  = channel->mPositionKeys[positionIndex].mTime;
            KeyPosition data;
            data.position  = GPE::toVec(aiPosition);
            data.timeStamp = timeStamp;
            m_Positions.push_back(data);
        }

        m_NumRotations = channel->mNumRotationKeys;
        for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
        {
            aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
            float        timeStamp     = channel->mRotationKeys[rotationIndex].mTime;
            KeyRotation  data;
            data.orientation = GPE::toQuat(aiOrientation);
            data.timeStamp   = timeStamp;
            m_Rotations.push_back(data);
        }

        m_NumScalings = channel->mNumScalingKeys;
        for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
        {
            aiVector3D scale     = channel->mScalingKeys[keyIndex].mValue;
            float      timeStamp = channel->mScalingKeys[keyIndex].mTime;
            KeyScale   data;
            data.scale     = GPE::toVec(scale);
            data.timeStamp = timeStamp;
            m_Scales.push_back(data);
        }
    }

    /* Interpolates b/w positions,rotations & scaling keys based on the curren time of the
    animation and prepares the local transformation matrix by combining all keys tranformations */
    void Update(float animationTime)
    {
        GPM::Mat4 translation = InterpolatePosition(animationTime);
        GPM::Mat4 rotation    = InterpolateRotation(animationTime);
        GPM::Mat4 scale       = InterpolateScaling(animationTime);
        m_LocalTransform      = translation * rotation * scale;
    }

    GPM::Mat4 GetLocalTransform()
    {
        return m_LocalTransform;
    }
    std::string GetBoneName() const
    {
        return m_Name;
    }
    int GetBoneID()
    {
        return m_ID;
    }

    /* Gets the current index on mKeyPositions to interpolate to based on the current
    animation time */
    int GetPositionIndex(float animationTime)
    {
        for (int index = 0; index < m_NumPositions - 1; ++index)
        {
            if (animationTime < m_Positions[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

    /* Gets the current index on mKeyRotations to interpolate to based on the current
    animation time */
    int GetRotationIndex(float animationTime)
    {
        for (int index = 0; index < m_NumRotations - 1; ++index)
        {
            if (animationTime < m_Rotations[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

    /* Gets the current index on mKeyScalings to interpolate to based on the current
    animation time */
    int GetScaleIndex(float animationTime)
    {
        for (int index = 0; index < m_NumScalings - 1; ++index)
        {
            if (animationTime < m_Scales[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

private:
    /* Gets normalized value for Lerp & Slerp*/
    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor  = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff   = nextTimeStamp - lastTimeStamp;
        scaleFactor        = midWayLength / framesDiff;
        return scaleFactor;
    }

    /* figures out which position keys to interpolate b/w and performs the interpolation
    and returns the translation matrix */
    GPM::Mat4 InterpolatePosition(float animationTime)
    {
        if (1 == m_NumPositions)
            return GPM::Transform::translation(m_Positions[0].position);

        int   p0Index = GetPositionIndex(animationTime);
        int   p1Index = p0Index + 1;
        float scaleFactor =
            GetScaleFactor(m_Positions[p0Index].timeStamp, m_Positions[p1Index].timeStamp, animationTime);
        GPM::Vec3 finalPosition = GPM::lerp(m_Positions[p0Index].position, m_Positions[p1Index].position, scaleFactor);
        return GPM::Transform::translation(finalPosition);
    }

    /* figures out which rotations keys to interpolate b/w and performs the interpolation
    and returns the rotation matrix */
    GPM::Mat4 InterpolateRotation(float animationTime)
    {
        if (1 == m_NumRotations)
        {
            auto rotation = m_Rotations[0].orientation.normalized();
            return GPM::toMatrix4(rotation);
        }

        int   p0Index = GetRotationIndex(animationTime);
        int   p1Index = p0Index + 1;
        float scaleFactor =
            GetScaleFactor(m_Rotations[p0Index].timeStamp, m_Rotations[p1Index].timeStamp, animationTime);
        GPM::Quat finalRotation = m_Rotations[p0Index].orientation.nlerp(m_Rotations[p1Index].orientation, scaleFactor);
        finalRotation           = finalRotation.normalized();
        return GPM::toMatrix4(finalRotation);
    }

    /* figures out which scaling keys to interpolate b/w and performs the interpolation
    and returns the scale matrix */
    GPM::Mat4 InterpolateScaling(float animationTime)
    {
        if (1 == m_NumScalings)
            return GPM::Transform::scaling(m_Scales[0].scale);

        int       p0Index     = GetScaleIndex(animationTime);
        int       p1Index     = p0Index + 1;
        float     scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp, m_Scales[p1Index].timeStamp, animationTime);
        GPM::Vec3 finalScale  = GPM::lerp(m_Scales[p0Index].scale, m_Scales[p1Index].scale, scaleFactor);
        return GPM::Transform::scaling(finalScale);
    }
};

} // namespace