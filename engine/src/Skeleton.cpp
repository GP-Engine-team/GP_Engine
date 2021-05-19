#include "Engine/Resources/Skeleton.hpp"

#include <iostream>

void GPE::Skeleton::printHierarchy() const
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
            std::cout << fullName << std::endl;
        }
    }
}