#include "Engine/Resources/Mesh.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "GPM/Constants.hpp"
#include "GPM/Shape3D/AABB.hpp"
#include "GPM/Shape3D/Sphere.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Resources/Animation/Animation.hpp>
#include <Engine/Resources/Animation/Skin.hpp>
#include <Engine/ECS/Component/AnimationComponent.hpp>

#include <glad/glad.h>

#include <limits>
#include <filesystem>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

using namespace GPE;
using namespace GPM;

Mesh::CreateIndiceBufferArg Mesh::convert(Mesh::CreateContiguousVerticesArg& arg)
{
    CreateIndiceBufferArg newArg;
    newArg.boundingVolumeType = arg.boundingVolumeType;

    for (size_t i = 0u; i < arg.iBuffer.size(); ++i)
    {
        newArg.vertices.push_back(
            {arg.vBuffer[arg.iBuffer[i].iv], arg.vnBuffer[arg.iBuffer[i].ivn], arg.vtBuffer[arg.iBuffer[i].ivt]});

        newArg.indices.push_back((unsigned int)i);
    }

    // Compute tangeantes
    Vec3  edge1, edge2, tangent, deltaUV1, deltaUV2;
    float f;
    for (size_t i = 0u; i < arg.iBuffer.size(); i += 3)
    {
        // calculate tangent/bitangent vectors of both triangles
        edge1    = newArg.vertices[i + 1].v - newArg.vertices[i].v;
        edge2    = newArg.vertices[i + 2].v - newArg.vertices[i].v;
        deltaUV1 = newArg.vertices[i + 1].vt - newArg.vertices[i].vt;
        deltaUV2 = newArg.vertices[i + 2].vt - newArg.vertices[i].vt;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x                  = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y                  = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z                  = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        newArg.vertices[i].vtg     = tangent;
        newArg.vertices[i + 1].vtg = tangent;
        newArg.vertices[i + 2].vtg = tangent;
    }

    return newArg;
}

Mesh::Mesh(Mesh::CreateIndiceBufferArg& arg) noexcept
{
    if (arg.boundingVolumeType != Mesh::EBoundingVolume::NONE)
    {
        Vec3 minAABB, maxAABB;
        generateAABB(arg.vertices, minAABB, maxAABB);
        generateBoundingVolume(arg.boundingVolumeType, minAABB, maxAABB);
    }

    m_verticesCount = static_cast<unsigned int>(arg.indices.size());

    // Generate buffer and bind VAO
    glGenVertexArrays(1, &m_buffers.vao);
    glGenBuffers(2, &m_buffers.vbo);
    glBindVertexArray(m_buffers.vao);

    // define properties of EBO and VBO buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arg.vertices[0]) * arg.vertices.size(), arg.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(arg.indices[0]) * arg.indices.size(), arg.indices.data(),
                 GL_STATIC_DRAW);

    // 1rst attribute buffer : position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(arg.vertices[0]), (GLvoid*)offsetof(Vertex, v));

    // 2nd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(arg.vertices[0]), (GLvoid*)offsetof(Vertex, vn));

    // 3th attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(arg.vertices[0]), (GLvoid*)offsetof(Vertex, vt));

    // 4th attribute buffer : Tangeantes
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(arg.vertices[0]), (GLvoid*)offsetof(Vertex, vtg));

    glBindVertexArray(0);

    Log::getInstance()->log("Mesh load in GPU with EBO");
}

Mesh::~Mesh() noexcept
{
    removeBoundingVolume();

    glDeleteBuffers(3, &m_buffers.vao);
}

void Mesh::draw() const noexcept
{
    glBindVertexArray(m_buffers.vao);

    unsigned int first = 0;
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_verticesCount));
}

void Mesh::setBoundingVolume(EBoundingVolume boundingVolumeType) noexcept
{
    std::vector<Vertex> posBuffer;
    Vec3                minAABB, maxAABB;

    getData(posBuffer);
    generateAABB(posBuffer, minAABB, maxAABB);
    generateBoundingVolume(boundingVolumeType, minAABB, maxAABB);
}

void Mesh::getData(std::vector<Vertex>& buffer)
{
    buffer.clear();

    GLint64 bufSize;
    glGetNamedBufferParameteri64v(m_buffers.vbo, GL_BUFFER_SIZE, &bufSize);

    size_t elemCount = bufSize / sizeof(Vertex);

    buffer.reserve(elemCount);
    for (size_t i = 0; i < elemCount; ++i)
        buffer.emplace_back();

    glGetNamedBufferSubData(m_buffers.vbo, 0, bufSize, buffer.data());
}

void Mesh::generateAABB(const std::vector<Vertex>& vertices, Vector3& minAABB, Vector3& maxAABB) noexcept
{
    minAABB = Vec3(std::numeric_limits<float>::max());
    maxAABB = Vec3(std::numeric_limits<float>::min());
    for (auto&& v : vertices)
    {
        minAABB.x = std::min(minAABB.x, v.v.x);
        minAABB.y = std::min(minAABB.y, v.v.y);
        minAABB.z = std::min(minAABB.z, v.v.z);

        maxAABB.x = std::max(maxAABB.x, v.v.x);
        maxAABB.y = std::max(maxAABB.y, v.v.y);
        maxAABB.z = std::max(maxAABB.z, v.v.z);
    }
}

void Mesh::removeBoundingVolume()
{
    switch (m_boundingVolumeType)
    {
    case Mesh::EBoundingVolume::SPHERE: {
        Engine::getInstance()->resourceManager.remove<Sphere>(std::to_string((size_t)this));
        break;
    }

    case Mesh::EBoundingVolume::AABB: {
        Engine::getInstance()->resourceManager.remove<AABB>(std::to_string((size_t)this));
        break;
    }
    default:
        break;
    }
    m_boundingVolume     = nullptr;
    m_boundingVolumeType = EBoundingVolume::NONE;
}

void Mesh::generateBoundingVolume(EBoundingVolume boundingVolumeType, const Vector3& minAABB,
                                  const Vector3& maxAABB) noexcept
{
    removeBoundingVolume();
    m_boundingVolumeType = boundingVolumeType;
    switch (boundingVolumeType)
    {
    case Mesh::EBoundingVolume::SPHERE: {
        m_boundingVolume = &Engine::getInstance()->resourceManager.add<Sphere>(
            std::to_string((size_t)this), std::max(minAABB.length(), maxAABB.length()), (maxAABB + minAABB) * 0.5);
        break;
    }

    case Mesh::EBoundingVolume::AABB: {
        m_boundingVolume =
            &Engine::getInstance()->resourceManager.add<AABB>(std::to_string((size_t)this), minAABB, maxAABB);
        break;
    }
    default:
        break;
    }
}

Mesh::CreateIndiceBufferArg Mesh::createQuad(float halfWidth, float halfHeight, float textureRepetition,
                                             unsigned int indexTextureX, unsigned int indexTextureY, Axis towardAxis,
                                             bool isRectoVerso) noexcept
{
    Mesh::CreateContiguousVerticesArg mesh;

    // plane contain 4 triangle, 4 vertex 4 texture coordonate and 2 normal
    mesh.vBuffer.reserve(4);
    mesh.vtBuffer.reserve(4);
    mesh.vnBuffer.reserve(isRectoVerso ? 2 : 1);
    mesh.iBuffer.reserve(isRectoVerso ? 12 : 6);

    // Face 1
    mesh.iBuffer.emplace_back(Indice{0, 0, 0});
    mesh.iBuffer.emplace_back(Indice{1, 1, 0});
    mesh.iBuffer.emplace_back(Indice{3, 2, 0});
    mesh.iBuffer.emplace_back(Indice{1, 1, 0});
    mesh.iBuffer.emplace_back(Indice{2, 3, 0});
    mesh.iBuffer.emplace_back(Indice{3, 2, 0});

    // initialize vertex :

    switch (towardAxis)
    {
    case Axis::X:
        mesh.vBuffer.push_back({0.f, -halfHeight, -halfWidth});
        mesh.vBuffer.push_back({0.f, halfHeight, -halfWidth});
        mesh.vBuffer.push_back({0.f, halfHeight, halfWidth});
        mesh.vBuffer.push_back({0.f, -halfHeight, halfWidth});
        mesh.vnBuffer.push_back({1.f, 0.f, 0.f});
        break;

    case Axis::NEG_X:
        mesh.vBuffer.push_back({0.f, halfHeight, halfWidth});
        mesh.vBuffer.push_back({0.f, -halfHeight, halfWidth});
        mesh.vBuffer.push_back({0.f, -halfHeight, -halfWidth});
        mesh.vBuffer.push_back({0.f, halfHeight, -halfWidth});
        mesh.vnBuffer.push_back({-1.f, 0.f, 0.f});
        break;

    case Axis::Y:
        mesh.vBuffer.push_back({halfHeight, 0.f, halfWidth});   // 0
        mesh.vBuffer.push_back({halfHeight, 0.f, -halfWidth});  // 1
        mesh.vBuffer.push_back({-halfHeight, 0.f, -halfWidth}); // 2
        mesh.vBuffer.push_back({-halfHeight, 0.f, halfWidth});  // 3
        mesh.vnBuffer.push_back({0.f, 1.f, 0.f});
        break;

    case Axis::NEG_Y:
        mesh.vBuffer.push_back({-halfHeight, 0.f, -halfWidth});
        mesh.vBuffer.push_back({halfHeight, 0.f, -halfWidth});
        mesh.vBuffer.push_back({halfHeight, 0.f, halfWidth});
        mesh.vBuffer.push_back({-halfHeight, 0.f, halfWidth});
        mesh.vnBuffer.push_back({0.f, -1.f, 0.f});
        break;

    case Axis::Z:
        mesh.vBuffer.push_back({-halfHeight, -halfWidth, 0.f});
        mesh.vBuffer.push_back({halfHeight, -halfWidth, 0.f});
        mesh.vBuffer.push_back({halfHeight, halfWidth, 0.f});
        mesh.vBuffer.push_back({-halfHeight, halfWidth, 0.f});
        mesh.vnBuffer.push_back({0.f, 0.f, 1.f});
        break;

    case Axis::NEG_Z:
        mesh.vBuffer.push_back({-halfHeight, -halfWidth, 0.f});
        mesh.vBuffer.push_back({halfHeight, -halfWidth, 0.f});
        mesh.vBuffer.push_back({halfHeight, halfWidth, 0.f});
        mesh.vBuffer.push_back({-halfHeight, halfWidth, 0.f});

        mesh.vnBuffer.push_back({0.f, 0.f, -1.f});
        break;

    default:
        FUNCT_WARNING(std::string("Other axis not implemented"))
        break;
    }

    // initialize texture coord :
    float shiftX = indexTextureX * textureRepetition;
    float shiftY = indexTextureY * textureRepetition;
    mesh.vtBuffer.push_back({shiftX, shiftY});
    mesh.vtBuffer.push_back({shiftX + textureRepetition, shiftY});
    mesh.vtBuffer.push_back({shiftX, shiftY + textureRepetition});
    mesh.vtBuffer.push_back({shiftX + textureRepetition, shiftY + textureRepetition});

    if (isRectoVerso)
    {
        // Face 2
        mesh.iBuffer.emplace_back(Indice{0, 0, 1});
        mesh.iBuffer.emplace_back(Indice{1, 1, 1});
        mesh.iBuffer.emplace_back(Indice{3, 2, 1});
        mesh.iBuffer.emplace_back(Indice{1, 1, 1});
        mesh.iBuffer.emplace_back(Indice{2, 3, 1});
        mesh.iBuffer.emplace_back(Indice{3, 2, 1});

        // normal
        mesh.vnBuffer.push_back({0.f, 1.f, 0.f});
    }

    return convert(mesh);
}

Mesh::CreateIndiceBufferArg Mesh::createCube(float textureRepetition) noexcept
{
    Mesh::CreateContiguousVerticesArg mesh;

    // cube contain 12 triangle, 8 vertex 4 texture coordonate and 6 normal
    mesh.vBuffer.reserve(8);
    mesh.vtBuffer.reserve(4);
    mesh.vnBuffer.reserve(6);
    mesh.iBuffer.reserve(6);

    // initialize the index of cube :
    // Face 1
    mesh.iBuffer.emplace_back(Indice{0, 0, 0});
    mesh.iBuffer.emplace_back(Indice{1, 1, 0});
    mesh.iBuffer.emplace_back(Indice{2, 2, 0});
    mesh.iBuffer.emplace_back(Indice{2, 2, 0});
    mesh.iBuffer.emplace_back(Indice{1, 1, 0});
    mesh.iBuffer.emplace_back(Indice{3, 3, 0});

    // Face 2
    mesh.iBuffer.emplace_back(Indice{2, 0, 1});
    mesh.iBuffer.emplace_back(Indice{3, 1, 1});
    mesh.iBuffer.emplace_back(Indice{4, 2, 1});
    mesh.iBuffer.emplace_back(Indice{4, 2, 1});
    mesh.iBuffer.emplace_back(Indice{3, 1, 1});
    mesh.iBuffer.emplace_back(Indice{5, 3, 1});

    // Face 3
    mesh.iBuffer.emplace_back(Indice{4, 3, 2});
    mesh.iBuffer.emplace_back(Indice{5, 2, 2});
    mesh.iBuffer.emplace_back(Indice{6, 1, 2});
    mesh.iBuffer.emplace_back(Indice{6, 1, 2});
    mesh.iBuffer.emplace_back(Indice{5, 2, 2});
    mesh.iBuffer.emplace_back(Indice{7, 0, 2});

    // Face 4
    mesh.iBuffer.emplace_back(Indice{6, 0, 3});
    mesh.iBuffer.emplace_back(Indice{7, 1, 3});
    mesh.iBuffer.emplace_back(Indice{0, 2, 3});
    mesh.iBuffer.emplace_back(Indice{0, 2, 3});
    mesh.iBuffer.emplace_back(Indice{7, 1, 3});
    mesh.iBuffer.emplace_back(Indice{1, 3, 3});

    // Face 5
    mesh.iBuffer.emplace_back(Indice{1, 0, 4});
    mesh.iBuffer.emplace_back(Indice{7, 1, 4});
    mesh.iBuffer.emplace_back(Indice{3, 2, 4});
    mesh.iBuffer.emplace_back(Indice{3, 2, 4});
    mesh.iBuffer.emplace_back(Indice{7, 1, 4});
    mesh.iBuffer.emplace_back(Indice{5, 3, 4});

    // Face 6
    mesh.iBuffer.emplace_back(Indice{6, 0, 5});
    mesh.iBuffer.emplace_back(Indice{0, 1, 5});
    mesh.iBuffer.emplace_back(Indice{4, 2, 5});
    mesh.iBuffer.emplace_back(Indice{4, 2, 5});
    mesh.iBuffer.emplace_back(Indice{0, 1, 5});
    mesh.iBuffer.emplace_back(Indice{2, 3, 5});

    // initialize vertex :
    mesh.vBuffer.push_back({-0.5f, -0.5f, 0.5});
    mesh.vBuffer.push_back({0.5f, -0.5f, 0.5});
    mesh.vBuffer.push_back({-0.5f, 0.5f, 0.5});
    mesh.vBuffer.push_back({0.5f, 0.5f, 0.5});
    mesh.vBuffer.push_back({-0.5f, 0.5f, -0.5});
    mesh.vBuffer.push_back({0.5f, 0.5f, -0.5});
    mesh.vBuffer.push_back({-0.5f, -0.5f, -0.5});
    mesh.vBuffer.push_back({0.5f, -0.5f, -0.5});

    // initialize texture coord :
    mesh.vtBuffer.push_back({0.f, 0.f});
    mesh.vtBuffer.push_back({textureRepetition, 0.f});
    mesh.vtBuffer.push_back({0.f, textureRepetition});
    mesh.vtBuffer.push_back({textureRepetition, textureRepetition});

    // initialize normal :
    mesh.vnBuffer.push_back({0.f, 0.f, 1.f});
    mesh.vnBuffer.push_back({0.f, 1.f, 0.f});
    mesh.vnBuffer.push_back({0.f, 0.f, -1.f});
    mesh.vnBuffer.push_back({0.f, -1.f, 0.f});
    mesh.vnBuffer.push_back({1.f, 0.f, 0.f});
    mesh.vnBuffer.push_back({-1.f, 0.f, 0.f});

    return convert(mesh);
}

Mesh::CreateIndiceBufferArg Mesh::createSphere(int latitudeCount, int longitudeCount) noexcept
{
    GPE_ASSERT(latitudeCount > 2 && longitudeCount > 2, "Latitude and Longitude must be greater than 2");

    Mesh::CreateContiguousVerticesArg mesh;

    latitudeCount *= 2;

    float latitudeStep  = 2.f * PI / latitudeCount;
    float longitudeStep = PI / longitudeCount;

    // found each point of sphere in function of latitude and longitude count in parameter
    // souce to more informaiton : http://www.songho.ca/opengl/gl_sphere.html
    {
        float xy;
        float latitudeAngle, longitudeAngle;
        float radius = 0.5f;    // radius of 1
        float posX, posY, posZ; // position of point
        float s, t;             // few constant for texture position

        for (unsigned int i = 0; i <= static_cast<unsigned int>(longitudeCount); i++)
        {
            longitudeAngle = PI / 2.f - i * longitudeStep; // starting from pi/2 to -pi/2
            xy             = radius * cosf(longitudeAngle);
            posZ           = radius * sinf(longitudeAngle);

            for (unsigned int j = 0; j <= static_cast<unsigned int>(latitudeCount); j++)
            {
                latitudeAngle = j * latitudeStep; // starting from 0 to 2pi
                posX          = xy * cosf(latitudeAngle);
                posY          = xy * sinf(latitudeAngle);
                s             = (float)j / latitudeCount;
                t             = (float)i / longitudeCount;

                // vertex position (x, y, z)
                mesh.vBuffer.push_back({posX, posY, posZ});
                mesh.vnBuffer.push_back({posX * radius, posY * radius, posZ * radius});
                mesh.vtBuffer.push_back({s, t});

                mesh.vnBuffer.back().normalize();
            }
        }
    }

    // generate each indices in function of point create upper
    {
        unsigned int ver1, ver2; // vertices with differente longitude. ver2 = longitude +1

        for (unsigned int i = 0; i < static_cast<unsigned int>(longitudeCount); i++)
        {
            ver1 = i * (latitudeCount + 1);
            ver2 = ver1 + latitudeCount + 1; // ver2 = longitude + 1

            for (unsigned int j = 0; j < static_cast<unsigned int>(latitudeCount); j++, ver1++, ver2++)
            {
                if (i != 0)
                {
                    // ver1 -> ver2 -> ver1+1 : like this shema
                    // ver1	 <-	ver1+1
                    //	|	   /
                    // ver2

                    // to create triangle like shema
                    mesh.iBuffer.push_back(Indice{ver1, ver1, ver1});
                    mesh.iBuffer.push_back(Indice{ver2, ver2, ver2});
                    mesh.iBuffer.push_back(Indice{ver1 + 1, ver1 + 1, ver1 + 1});
                }

                if (i != static_cast<unsigned int>(longitudeCount) - 1)
                {
                    // ver1+1 -> ver2 -> ver2+1 : like this shema
                    //			ver1+1
                    //		 /	  |
                    // ver2	 ->	ver2+1

                    mesh.iBuffer.push_back(Indice{ver1 + 1, ver1 + 1, ver1 + 1});
                    mesh.iBuffer.push_back(Indice{ver2, ver2, ver2});
                    mesh.iBuffer.push_back(Indice{ver2 + 1, ver2 + 1, ver2 + 1});
                }
            }
        }
    }

    return convert(mesh);
}

Mesh::CreateIndiceBufferArg Mesh::createCylindre(unsigned int prescision) noexcept
{
    GPE_ASSERT(prescision > 2, "Prescision must be greater than 2");

    Mesh::CreateContiguousVerticesArg mesh;

    // Cylindre contain prescision * 2 + 2
    mesh.vBuffer.reserve(static_cast<size_t>(prescision) * 2 + 2);
    mesh.vnBuffer.reserve(static_cast<size_t>(prescision) * 2 + 2);
    mesh.vtBuffer.reserve(static_cast<size_t>(prescision) * 2 + 2);

    float angleRad = PI * 2 / static_cast<float>(prescision);

    // near face

    // middle of near face is in front
    mesh.vBuffer.push_back({0.f, 0.f, 0.5f});
    mesh.vnBuffer.push_back({0.f, 0.f, 1.f});
    mesh.vtBuffer.push_back({0.f, 1.f});

    for (unsigned int i = 0; i < prescision; i++)
    {
        mesh.vBuffer.push_back({0.5f * cosf(i * angleRad), 0.5f * sinf(i * angleRad), 0.5f});
        mesh.vnBuffer.push_back({cosf(i * angleRad), sinf(i * angleRad), 1.f});
        mesh.vtBuffer.push_back({0.f, 1.f});

        mesh.vnBuffer.back().normalize();
    }

    // far face
    for (unsigned int i = 0; i < prescision; i++)
    {
        mesh.vBuffer.push_back({0.5f * cosf(i * angleRad), 0.5f * sinf(i * angleRad), -0.5f});
        mesh.vnBuffer.push_back({cosf(i * angleRad), sinf(i * angleRad), -1.f});
        mesh.vtBuffer.push_back({0.f, 1.f});

        mesh.vnBuffer.back().normalize();
    }

    // middle of far face is in back
    mesh.vBuffer.push_back({0.f, 0.f, -0.5f});
    mesh.vnBuffer.push_back({0.f, 0.f, -1.f});
    mesh.vtBuffer.push_back({0.f, 1.f});

    // calcul indice of mesh :

    // near face triangle indice
    for (unsigned int i = 1; i < prescision; i++)
    {
        mesh.iBuffer.push_back(Indice{0, 0, 0});
        mesh.iBuffer.push_back(Indice{i, i, i});
        mesh.iBuffer.push_back(Indice{i + 1, i + 1, i + 1});
    }

    mesh.iBuffer.push_back(Indice{0, 0, 0});
    mesh.iBuffer.push_back(Indice{prescision, prescision, prescision});
    mesh.iBuffer.push_back(Indice{1, 1, 1});

    // side face triangle indice
    for (unsigned int i = 1; i < prescision; i++)
    {
        // face is blit in 2 triangle :
        mesh.iBuffer.push_back(Indice{i, i, i});
        mesh.iBuffer.push_back(Indice{i + 1, i + 1, i + 1});
        mesh.iBuffer.push_back(Indice{i + prescision, i + prescision, i + prescision});

        // triangle 2 :
        mesh.iBuffer.push_back(Indice{i + prescision, i + prescision, i + prescision});
        mesh.iBuffer.push_back(Indice{i + prescision + 1, i + prescision + 1, i + prescision + 1});
        mesh.iBuffer.push_back(Indice{i + 1, i + 1, i + 1});
    }

    // face is blit in 2 triangle :
    mesh.iBuffer.push_back(Indice{prescision, prescision, prescision});
    mesh.iBuffer.push_back(Indice{1, 1, 1});
    mesh.iBuffer.push_back(Indice{prescision + prescision, prescision + prescision, prescision + prescision});

    // triangle 2 :
    mesh.iBuffer.push_back(Indice{prescision + prescision, prescision + prescision, prescision + prescision});
    mesh.iBuffer.push_back(Indice{prescision + 1, prescision + 1, prescision + 1});
    mesh.iBuffer.push_back(Indice{1, 1, 1});

    unsigned int middleFarPointIndice = prescision * 2 + 1;

    // far face triangle indice
    for (unsigned int i = prescision + 1; i < prescision + prescision; i++)
    {
        mesh.iBuffer.push_back(Indice{middleFarPointIndice, middleFarPointIndice, middleFarPointIndice});
        mesh.iBuffer.push_back(Indice{i, i, i});
        mesh.iBuffer.push_back(Indice{i + 1, i + 1, i + 1});
    }

    mesh.iBuffer.push_back(Indice{middleFarPointIndice, middleFarPointIndice, middleFarPointIndice});
    mesh.iBuffer.push_back(Indice{prescision + prescision, prescision + prescision, prescision + prescision});
    mesh.iBuffer.push_back(Indice{prescision + 1, prescision + 1, prescision + 1});

    return convert(mesh);
}

void Mesh::bindSkin(Skin& skin)
{
    glBindVertexArray(m_buffers.vao);

    skin.uploadBufferData();

    skin.setVertexAttribArray(4, 5);

    glBindVertexArray(0);
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, Mesh::Vertex& inspected)
{
    inspect(context, inspected.v, "v");
    inspect(context, inspected.vn, "vn");
    inspect(context, inspected.vt, "uv");
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, Mesh::Indice& inspected)
{
    inspect(context, inspected.iv, "iv");
    inspect(context, inspected.ivn, "ivn");
    inspect(context, inspected.ivt, "iuv");
}