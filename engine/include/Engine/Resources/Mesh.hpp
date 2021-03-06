﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/Resources/Type.hpp>
#include <Engine/Serialization/DataInspector.hpp>
#include <GPM/Shape3D/AABB.hpp>
#include <GPM/Shape3D/Sphere.hpp>
#include <GPM/Shape3D/Volume.hpp>
#include <GPM/Vector3.hpp>

struct aiScene;
struct aiMesh;

namespace GPE
{
class Mesh
{
public:
    enum class EBoundingVolume
    {
        NONE   = 0,
        SPHERE = 1,
        AABB   = 2,
        COUNT
    };

    struct Vertex
    {
        GPM::Vec3 v;   // position
        GPM::Vec3 vn;  // normal
        GPM::Vec2 vt;  // UV
        GPM::Vec3 vtg; // tangeante
    };

    struct Indice
    {
        unsigned int iv, ivt, ivn;
    };

    // Allow user to construct mesh thank's to EBO
    struct CreateIndiceBufferArg
    {
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        EBoundingVolume           boundingVolumeType{EBoundingVolume::NONE};
    };

    // Allow user to construct mesh thank's to multiple VBO
    struct CreateContiguousVerticesArg
    {
        std::vector<GPM::Vec3> vBuffer;
        std::vector<GPM::Vec2> vtBuffer;
        std::vector<GPM::Vec3> vnBuffer;
        std::vector<Indice>    iBuffer; // optional
        EBoundingVolume        boundingVolumeType{EBoundingVolume::NONE};
    };

    enum class Axis
    {
        X,
        NEG_X,
        Y,
        NEG_Y,
        Z,
        NEG_Z,
    };

protected:
    unsigned int m_verticesCount = 0;

    struct
    {
        unsigned int vao = 0;
        unsigned int vbo = 0;
        unsigned int ebo = 0;
    } m_buffers;

    EBoundingVolume m_boundingVolumeType = EBoundingVolume::NONE;
    GPM::Volume*    m_boundingVolume     = nullptr;

    // Local AABB Attribut
    GPM::Vec3 m_minAABB, m_maxAABB;

protected:
    void removeBoundingVolume();

public:
    Mesh(CreateIndiceBufferArg& arg) noexcept;
    Mesh(CreateIndiceBufferArg&& arg) noexcept;

    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other)      = default;
    ~Mesh() noexcept;

    /**
     * @brief Drawn Mesh
     *
     */
    void draw() const noexcept;

    inline const GPM::Volume* getBoundingVolume() const noexcept;

    /**
     * @brief Recompute the bounding volume without conciderate the previous volume.
     * This function can be slow because vertice will be get from GPU.
     * The mesh must also be loaded in GPU to use this funciton.
     */
    void setBoundingVolume(EBoundingVolume boundingVolumeType) noexcept;

    /**
     * @brief This function will obtain the vertices data from the GPU. This operation can be slow.
     * Warning : data retuned is only the buffer data and not the vertice buffer. Indice is not used to sorte the
     * returned buffer
     * @return
     */
    void getData(std::vector<Vertex>& buffer);

    GETTER_BY_VALUE(ID, m_buffers.vao);
    GETTER_BY_VALUE(VerticesCount, m_verticesCount);
    GETTER_BY_VALUE(BoundingVolumeType, m_boundingVolumeType);

    static CreateIndiceBufferArg convert(CreateContiguousVerticesArg& arg);

    /**
     * @brief Create a plae object of radius 1 and return it mesh. Plane is centered on the origin
     *
     * @param textureRepetition     : if > 1 : repetition else split
     * @param indexTexture          : index of texture if split
     * @return MeshConstructorArg
     */
    static CreateIndiceBufferArg createQuad(float halfWidth = 0.5f, float halfHeight = 0.5f,
                                            float textureRepetition = 1.f, unsigned int indexTextureX = 0,
                                            unsigned int indexTextureY = 0, Axis towardAxis = Axis::Y,
                                            bool isRectoVerso = false) noexcept;

    /**
     * @brief Create a Cube object of radius 1 and return it mesh. Cube is centered on the origin
     *
     * @return MeshConstructorArg
     */
    static CreateIndiceBufferArg createCube(float textureRepetition = 1.f) noexcept;

    /**
     * @brief Create a Sphere object of radius 1 and return it mesh. Sphere is centered on the origin
     *
     * @param latitudeCount     : number of vertex in latitude
     * @param longitudeCount    : number of vertex in longitude
     * @return MeshConstructorArg
     */
    static CreateIndiceBufferArg createSphere(int latitudeCount, int longitudeCount) noexcept;

    /**
     * @brief Create a Cylindre object
     *
     * @param prescision
     * @return MeshConstructorArg
     */
    static CreateIndiceBufferArg createCylindre(unsigned int prescision) noexcept; // TODO:: add uv and backFace
                                                                                   // Culling (bad
                                                                                   // normal)

    /**
     * @brief Generate AABB min and max value in function og list of vertices
     * @param vertices
     * @param minAABB
     * @param maxAABB
     * @return
     */
    static void generateAABB(const std::vector<Vertex>& vertices, GPM::Vec3& minAABB, GPM::Vec3& maxAABB) noexcept;

    /**
     * @brief Generate bounding volume in function of min and max AABB found.
     * User can use function generateAABB to found this value in function of vertices
     * @param boundingVolumeType
     * @param minAABB
     * @param maxAABB
     * @return
     */
    void generateBoundingVolume(EBoundingVolume boundingVolumeType, const GPM::Vec3& minAABB,
                                const GPM::Vec3& maxAABB) noexcept;

    void bindSkin(class Skin& skin);

    /**
     * @brief Function to return the local AABB (do not considere the position, scale and rotation of transform)
     * @return
     */
    GPM::AABB        getAABB();
    const GPM::Vec3& getAABBMin();
    const GPM::Vec3& getAABBMAx();
};

template <>
void DataInspector::inspect(GPE::InspectContext& context, Mesh::Vertex& inspected);

template <>
void DataInspector::inspect(GPE::InspectContext& context, Mesh::Indice& inspected);

} /*namespace GPE*/

#include "Mesh.inl"
