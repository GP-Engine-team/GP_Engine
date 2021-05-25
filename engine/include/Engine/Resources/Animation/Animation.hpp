#pragma once

#include <GPM/Matrix4.hpp>
#include <Engine/ECS/Component/Model.hpp>
#include <Engine/Resources/Animation/Bone.hpp>
#include <Engine/Resources/Mesh.hpp>
#include <Engine/Resources/Importer/AssimpUtilities.hpp>
#include <vector>
#include <map>
#include <string>
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

namespace GPE
{

struct AssimpNodeData
{
    GPM::Matrix4                transformation;
    std::string                 name;
    int                         childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
public:
    Animation() = default;

    Animation(const std::string& animationPath, Mesh* mesh)
    {
        Assimp::Importer importer;
        const aiScene*   scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);
        auto animation   = scene->mAnimations[0];
        m_Duration       = animation->mDuration;
        m_TicksPerSecond = animation->mTicksPerSecond;
        ReadHeirarchyData(m_RootNode, scene->mRootNode);
        ReadMissingBones(animation, *mesh);
    }

    ~Animation()
    {
    }

    Bone* FindBone(const std::string& name)
    {
        auto iter =
            std::find_if(m_Bones.begin(), m_Bones.end(), [&](const Bone& Bone) { return Bone.GetBoneName() == name; });
        if (iter == m_Bones.end())
            return nullptr;
        else
            return &(*iter);
    }

    inline float GetTicksPerSecond()
    {
        return m_TicksPerSecond;
    }

    inline float GetDuration()
    {
        return m_Duration;
    }

    inline const AssimpNodeData& GetRootNode()
    {
        return m_RootNode;
    }

    inline const std::map<std::string, GPE::Mesh::BoneInfo>& GetBoneIDMap()
    {
        return m_BoneInfoMap;
    }

private:
    void ReadMissingBones(const aiAnimation* animation, Mesh& mesh)
    {
        int size = animation->mNumChannels;

        auto& boneInfoMap = mesh.m_BoneInfoMap; // getting m_BoneInfoMap from Model class
        int&  boneCount   = mesh.m_BoneCounter;   // getting the m_BoneCounter from Model class

        // reading channels(bones engaged in an animation and their keyframes)
        for (int i = 0; i < size; i++)
        {
            auto        channel  = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;

            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                boneInfoMap[boneName].id = boneCount;
                boneCount++;
            }
            m_Bones.push_back(Bone{std::string(channel->mNodeName.data), boneInfoMap[channel->mNodeName.data].id, channel});
        }

        m_BoneInfoMap = boneInfoMap;
    }

    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
    {
        assert(src);

        dest.name           = src->mName.data;
        dest.transformation = GPE::toMat4(src->mTransformation);
        dest.childrenCount  = src->mNumChildren;

        for (int i = 0; i < src->mNumChildren; i++)
        {
            AssimpNodeData newData;
            ReadHeirarchyData(newData, src->mChildren[i]);
            dest.children.push_back(newData);
        }
    }
    float                           m_Duration;
    int                             m_TicksPerSecond;
    std::vector<Bone>               m_Bones;
    AssimpNodeData                  m_RootNode;
    std::map<std::string, GPE::Mesh::BoneInfo> m_BoneInfoMap;
};

} // namespace GPE