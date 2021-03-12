﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Engine/Core/Tools/ClassUtility.hpp"
#include "Engine/Resources/Type.hpp"
#include "GPM/Shape3D/Volume.hpp"
#include "GPM/Vector3.hpp"

namespace GPE
{

class Mesh
{
public:
    enum class EBoundingVolume
    {
        NONE,
        SPHERE,
        AABB
    };

    struct Vertex
    {
        GPM::Vec3 vBuffer;
        GPM::Vec3 vnBuffer;
        GPM::Vec2 vtBuffer;
    };

    struct Indice
    {
        unsigned int iv, ivt, ivn;
    };

    // Allow user to construct mesh thank's to EBO
    struct CreateIndiceBufferArg
    {
        std::string                  objName;
        std::vector<Vertex>          vertices;
        std::vector<unsigned int>    indices;
        EBoundingVolume              boundingVolumeType{EBoundingVolume::SPHERE};
        std::unique_ptr<GPM::Volume> boundingVolume = nullptr;
    };

    // Allow user to construct mesh thank's to multiple VBO
    struct CreateContiguteVerticesArg
    {
        std::string                  objName;
        std::vector<GPM::Vec3>       vBuffer;
        std::vector<GPM::Vec2>       vtBuffer;
        std::vector<GPM::Vec3>       vnBuffer;
        std::vector<Indice>          iBuffer; // optional
        EBoundingVolume              boundingVolumeType{EBoundingVolume::SPHERE};
        std::unique_ptr<GPM::Volume> boundingVolume = nullptr;
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
    unsigned int m_VAO           = 0;
    unsigned int m_verticesCount = 0;

    EBoundingVolume              m_boundingVolumeType = EBoundingVolume::NONE;
    std::unique_ptr<GPM::Volume> m_boundingVolume     = nullptr;

public:
    Mesh(CreateIndiceBufferArg& arg) noexcept;
    Mesh(CreateContiguteVerticesArg& arg) noexcept;

    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other)      = default;
    ~Mesh() noexcept;

    /**
     * @brief Drawn Mesh
     *
     */
    void draw() const noexcept;

    inline const GPM::Volume* getBoundingVolume() const noexcept;

    GETTER_BY_VALUE(ID, m_VAO);
    GETTER_BY_VALUE(VerticesCount, m_verticesCount);
    DEFAULT_GETTER_SETTER_BY_REF(BoundingVolumeType, m_boundingVolumeType);

    /**
     * @brief Create a plae object of radius 1 and return it mesh. Plane is centered on the origin
     *
     * @param textureRepetition     : if > 1 : repetition else split
     * @param indexTexture          : index of texture if split
     * @return MeshConstructorArg
     */
    static CreateContiguteVerticesArg createQuad(float halfWidth = 0.5f, float halfHeight = 0.5f,
                                                 float textureRepetition = 1.f, unsigned int indexTextureX = 0,
                                                 unsigned int indexTextureY = 0, Axis towardAxis = Axis::Y,
                                                 bool isRectoVerso = false) noexcept;

    /**
     * @brief Create a Cube object of radius 1 and return it mesh. Cube is centered on the origin
     *
     * @return MeshConstructorArg
     */
    static CreateContiguteVerticesArg createCube(float textureRepetition = 1.f) noexcept;

    /**
     * @brief Create a Sphere object of radius 1 and return it mesh. Sphere is centered on the origin
     *
     * @param latitudeCount     : number of vertex in latitude
     * @param longitudeCount    : number of vertex in longitude
     * @return MeshConstructorArg
     */
    static CreateContiguteVerticesArg createSphere(int latitudeCount, int longitudeCount) noexcept;

    /**
     * @brief Create a Cylindre object
     *
     * @param prescision
     * @return MeshConstructorArg
     */
    static CreateContiguteVerticesArg createCylindre(unsigned int prescision) noexcept; // TODO:: add uv and backFace
                                                                                        // Culling (bad
                                                                                        // normal)
};

#include "Mesh.inl"

} /*namespace GPE*/
