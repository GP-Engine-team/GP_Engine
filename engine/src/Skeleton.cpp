#include "Engine/Resources/Skeleton.hpp"

void GPE::Skeleton::PrintHierarchy() const
{
    for (int boneIndex = 0; boneIndex < getNbBones(); boneIndex++)
    {
        const std::string& parentBoneName    = m_boneNames[boneIndex];
        std::string separator   = " -> ";
        int         parentIndex = boneIndex;

        while (m_parentBoneIndices[parentIndex] != -1)
        {
            parentIndex = m_parentBoneIndices[parentIndex];
            std::string fullName    = m_boneNames[parentIndex] + separator + parentBoneName;
        }
    }
}