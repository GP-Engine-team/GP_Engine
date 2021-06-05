/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

struct aiMesh;

namespace GPE
{
class Skeleton;

/**
 * @brief Info that links a skeleton's bones to a mesh's vertices
 */
class Skin
{
public:
    struct VertexBoneData
    {
        static constexpr size_t maxBoneInfluence = 4;

        // bone indices which will influence this vertex
        int m_boneIDs[maxBoneInfluence];

        // weights from each bone
        float m_weights[maxBoneInfluence];
    };

    struct CreateArgs
    {
        std::vector<VertexBoneData> m_verticesBoneData; // contains the weights of each vertex
    };

public:
    unsigned int                m_skinVbo = 0;
    std::vector<VertexBoneData> m_verticesBoneData; // contains the weights of each vertex

    // TODO : rvalue
    Skin(const CreateArgs& args) noexcept;
    Skin();
    Skin(const Skin&) = delete;
    Skin(Skin&&)      = delete;
    ~Skin();
    Skin& operator=(const Skin&) = delete;
    Skin& operator=(Skin&&) = delete;

    void setVertexAttribArray(int boneIDs, int weights);
    void uploadBufferData();

    static void setVertexBoneDataToDefault(VertexBoneData& vertexBoneData);
    static void setVertexBoneData(VertexBoneData& vertexBoneData, int boneID, float weight);
};

void loadSkinAndSkeleton(std::vector<GPE::Skin::VertexBoneData>&    verticesBoneData,
                         std::map<std::string, Skeleton::BoneInfo>& m_boneInfoMap, aiMesh* mesh);

} // namespace GPE