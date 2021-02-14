/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include "Engine/Resources/Type.hpp"

#include "GPM/Vector3.hpp"

namespace Engine::Resources
{

class Mesh
{
public:
    struct CreateArg
    {
        std::string objName;
        std::vector<GPM::Vec3> vBuffer;
        std::vector<GPM::Vec2> vtBuffer;
        std::vector<GPM::Vec3> vnBuffer;
        std::vector<std::vector<Engine::Resources::Indice>> iBuffer;
    };

    enum class Axis
    {
        X,
        Y,
        Z
    };

protected:
    GLuint m_indexVAO;
    bool m_isLoadInGPU;

    std::string m_objName;
    std::vector<GPM::Vec3> m_vBuffer;
    std::vector<GPM::Vec2> m_vtBuffer;
    std::vector<GPM::Vec3> m_vnBuffer;
    std::vector<std::vector<Engine::Resources::Indice>> m_iBuffer;
    std::vector<std::string> m_idMaterial;

private:
    // this function generate the normal of object. Must be use if obj file don't contain normal.
    void generateNormalAndLoadIndice(const std::vector<Shape>& shapes) noexcept;

public:
    //Mesh(const char* objPath, bool loadInGPU = true);
    Mesh(const CreateArg& meshArg, bool loadInGPU = true);
    Mesh(const Attrib& attrib, const std::vector<Shape>& shape, bool loadInGPU = true);

    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) = default;
    ~Mesh();

    /**
     * @brief Load Mesh from CPU to GPU. This operation can be slow but use more faster the Mesh.
     *
     */
    void loadInGPU() noexcept;
    void unloadFromGPU() noexcept;

    /**
     * @brief return true if Mesh is load in GPU and ready to use
     *
     * @return true
     * @return false
     */
    bool isLoadInGPU() const noexcept
    {
        return m_isLoadInGPU;
    }

    /**
     * @brief Drawn Mesh
     *
     */
    void draw() const noexcept;

    /**
     * @brief Create a plae object of radius 1 and return it mesh. Plane is centered on the origin
     *
     * @param textureRepetition     : if > 1 : repetition else split
     * @param indexTexture          : index of texture if split
     * @return MeshConstructorArg
     */
    static Mesh::CreateArg createPlane(float textureRepetition = 1.f, unsigned int indexTextureX = 0,
                                          unsigned int indexTextureY = 0, Axis towardAxis = Axis::Y);

    /**
     * @brief Create a Cube object of radius 1 and return it mesh. Cube is centered on the origin
     *
     * @return MeshConstructorArg
     */
    static CreateArg createCube(float textureRepetition = 1.f);

    /**
     * @brief Create a Sphere object of radius 1 and return it mesh. Sphere is centered on the origin
     *
     * @param latitudeCount     : number of vertex in latitude
     * @param longitudeCount    : number of vertex in longitude
     * @return MeshConstructorArg
     */
    static CreateArg createSphere(int latitudeCount, int longitudeCount);

    /**
     * @brief Create a Cylindre object
     *
     * @param prescision
     * @return MeshConstructorArg
     */
    static CreateArg createCylindre(unsigned int prescision); // TODO:: add uv and backFace Culling (bad
                                                              // normal)

    /**
     * @brief return the id of Mesh load in GPU.
     *
     * @return const GLuint* : nullptr if Mesh is not load in GPU
     */
    const GLuint* getVAOId() noexcept
    {
        return &m_indexVAO;
    }

    const std::vector<GPM::Vec3>& getVertices() const noexcept
    {
        return m_vBuffer;
    }
    const std::vector<GPM::Vec2>& getUV() const noexcept
    {
        return m_vtBuffer;
    }
    const std::vector<GPM::Vec3>& getNormals() const noexcept
    {
        return m_vnBuffer;
    }
    const std::vector<std::vector<Engine::Resources::Indice>>& getIndices() const noexcept
    {
        return m_iBuffer;
    }

    /**
     * @brief Get the Id Materials object
     *
     * @return const std::vector<std::string>&
     */
    const std::vector<std::string>& getIdMaterials() const noexcept
    {
        return m_idMaterial;
    }
};
} /*namespace Engine::Resources*/