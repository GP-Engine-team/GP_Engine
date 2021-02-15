#include "Engine/Resources/Mesh.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "GPM/Constants.hpp"

#include <iostream>
#include <limits>

using namespace GPM;
using namespace Engine::Core::Debug;
using namespace Engine::Resources;

Mesh::Mesh(const CreateArg& arg, bool isLoadInGPU)
    : m_indexVAO(0), m_isLoadInGPU(false), m_objName(arg.objName), m_vBuffer(arg.vBuffer), m_vtBuffer(arg.vtBuffer),
      m_vnBuffer(arg.vnBuffer), m_iBuffer(arg.iBuffer)
{
    if (isLoadInGPU)
        loadInGPU();
}

Mesh::Mesh(const Attrib& attrib, const std::vector<Shape>& shapes, bool isLoadInGPU)
    : m_indexVAO(0), m_isLoadInGPU(false), m_objName(attrib.objName), m_vBuffer(attrib.vBuffer),
      m_vtBuffer(attrib.vtBuffer), m_vnBuffer(attrib.vnBuffer), m_iBuffer(), m_idMaterial()
{
    m_iBuffer.reserve(shapes.size());
    m_idMaterial.reserve(shapes.size());

    if (m_vnBuffer.empty())
    {
        generateNormalAndLoadIndice(shapes);
    }
    else
    {
        for (auto&& shape : shapes)
        {
            m_iBuffer.push_back({});
            for (size_t i = 0; i < shape.iv.size(); i++)
            {
                m_iBuffer.back().push_back({shape.iv[i], shape.ivt[i], shape.ivn[i]});
            }

            if (!shape.material_ids.empty())
                m_idMaterial.push_back(shape.material_ids);
        }
    }

    if (isLoadInGPU)
        loadInGPU();
}

Mesh::~Mesh()
{
    if (m_isLoadInGPU)
        unloadFromGPU();
}

void initializeVertexBuffer(GLuint& buffer, GLenum target, GLenum usage, const void* data, int size)
{
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, size, data, usage);
    glBindBuffer(target, 0);
}

void Mesh::loadInGPU() noexcept
{
    if (m_isLoadInGPU)
    {
        FUNCT_WARNING((std::string("Mesh name's : ") + m_objName.c_str() + " already load in GPU").c_str());
        return;
    }

    std::vector<Vec3> vVBO;
    std::vector<Vec2> vtVBO;
    std::vector<Vec3> vnVBO;

    unsigned int nbVertex = 0;
    for (size_t part = 0; part < m_iBuffer.size(); part++)
    {
        nbVertex += static_cast<unsigned int>(m_iBuffer[part].size());
        ;
    }

    vVBO.reserve(nbVertex);
    vtVBO.reserve(nbVertex);
    vnVBO.reserve(nbVertex);

    for (size_t part = 0; part < m_iBuffer.size(); part++)
    {
        for (size_t i = 0; i < m_iBuffer[part].size(); i++)
        {
            vVBO.push_back(m_vBuffer[m_iBuffer[part][i].iv]);
            vtVBO.push_back(m_vtBuffer[m_iBuffer[part][i].ivt]);
            vnVBO.push_back(m_vnBuffer[m_iBuffer[part][i].ivn]);
        }
    }

    // Init VBOs and VAO
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    // GLuint EBOid;
    initializeVertexBuffer(vertexbuffer, GL_ARRAY_BUFFER, GL_STATIC_DRAW, &vVBO[0],
                           static_cast<int>(vVBO.size() * sizeof(Vec3)));
    initializeVertexBuffer(uvbuffer, GL_ARRAY_BUFFER, GL_STATIC_DRAW, &vtVBO[0],
                           static_cast<int>(vtVBO.size() * sizeof(Vec2)));
    initializeVertexBuffer(normalbuffer, GL_ARRAY_BUFFER, GL_STATIC_DRAW, &vnVBO[0],
                           static_cast<int>(vnVBO.size() * sizeof(Vec3)));

    // Generate VAO
    glGenVertexArrays(1, &m_indexVAO);
    glBindVertexArray(m_indexVAO);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 2nd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 3nd attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    m_isLoadInGPU = true;
    Log::log((std::string("Mesh ") + m_objName.c_str() + " load in GPU").c_str());
}

void Mesh::unloadFromGPU() noexcept
{
    if (!m_isLoadInGPU)
    {
        FUNCT_WARNING((std::string("Mesh name's : ") + m_objName.c_str() + " isn't load in GPU").c_str());
        return;
    }

    // search all VBO attach to the current VAO and destroy it.
    GLint nAttr = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttr);
    glBindVertexArray(m_indexVAO);
    for (int iAttr = 0; iAttr < nAttr; ++iAttr)
    {
        GLuint vboId = 0;
        glGetVertexAttribiv(iAttr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, (GLint*)&vboId);
        if (vboId > 0)
        {
            glDeleteBuffers(1, &vboId);
        }
    }

    glDeleteBuffers(1, &m_indexVAO);
    m_indexVAO = 0;

    m_isLoadInGPU = false;
}

void Mesh::draw() const noexcept
{
    glBindVertexArray(m_indexVAO);

    unsigned int first = 0;
    for (size_t part = 0; part < m_iBuffer.size(); part++)
    {
        glDrawArrays(GL_TRIANGLES, first, static_cast<GLsizei>(m_iBuffer[part].size()));
        first += static_cast<unsigned int>(m_iBuffer[part].size());
    }
}

Mesh::CreateArg Mesh::createPlane(float textureRepetition, unsigned int indexTextureX, unsigned int indexTextureY,
                                  Axis towardAxis)
{
    Mesh::CreateArg mesh;
    mesh.objName = "Plane";

    // plane contain 4 triangle, 4 vertex 4 texture coordonate and 2 normal
    mesh.vBuffer.reserve(4);
    mesh.vtBuffer.reserve(4);
    mesh.vnBuffer.reserve(2);
    mesh.iBuffer.push_back({});
    mesh.iBuffer.reserve(2);

    // Face 1
    mesh.iBuffer[0].emplace_back(Indice{0, 0, 0});
    mesh.iBuffer[0].emplace_back(Indice{1, 1, 0});
    mesh.iBuffer[0].emplace_back(Indice{3, 2, 0});
    mesh.iBuffer[0].emplace_back(Indice{1, 1, 0});
    mesh.iBuffer[0].emplace_back(Indice{2, 3, 0});
    mesh.iBuffer[0].emplace_back(Indice{3, 2, 0});

    // Face 2
    mesh.iBuffer[0].emplace_back(Indice{0, 0, 1});
    mesh.iBuffer[0].emplace_back(Indice{1, 1, 1});
    mesh.iBuffer[0].emplace_back(Indice{3, 2, 1});
    mesh.iBuffer[0].emplace_back(Indice{1, 1, 1});
    mesh.iBuffer[0].emplace_back(Indice{2, 3, 1});
    mesh.iBuffer[0].emplace_back(Indice{3, 2, 1});

    // initialize vertex :

    switch (towardAxis)
    {
    case Axis::X:
        mesh.vBuffer.push_back({0.f, -0.5f, -0.5});
        mesh.vBuffer.push_back({0.f, 0.5f, -0.5});
        mesh.vBuffer.push_back({0.f, 0.5f, 0.5});
        mesh.vBuffer.push_back({0.f, -0.5f, 0.5});
        break;

    case Axis::Y:
        mesh.vBuffer.push_back({-0.5f, 0.f, -0.5});
        mesh.vBuffer.push_back({0.5f, 0.f, -0.5});
        mesh.vBuffer.push_back({0.5f, 0.f, 0.5});
        mesh.vBuffer.push_back({-0.5f, 0.f, 0.5});
        break;

    case Axis::Z:
        mesh.vBuffer.push_back({-0.5f, -0.5, 0.f});
        mesh.vBuffer.push_back({0.5f, -0.5, 0.f});
        mesh.vBuffer.push_back({0.5f, 0.5, 0.f});
        mesh.vBuffer.push_back({-0.5f, 0.5, 0.f});
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

    // initialize normal :
    mesh.vnBuffer.push_back({0.f, -1.f, 0.f});
    mesh.vnBuffer.push_back({0.f, 1.f, 0.f});

    return mesh;
}

Mesh::CreateArg Mesh::createCube(float textureRepetition)
{
    Mesh::CreateArg mesh;
    mesh.objName = "Cube";

    // cube contain 12 triangle, 8 vertex 4 texture coordonate and 6 normal
    mesh.vBuffer.reserve(8);
    mesh.vtBuffer.reserve(4);
    mesh.vnBuffer.reserve(6);
    mesh.iBuffer.push_back({});
    mesh.iBuffer.reserve(6);

    // initialize the index of cube :
    // Face 1
    mesh.iBuffer[0].emplace_back(Indice{0, 0, 0});
    mesh.iBuffer[0].emplace_back(Indice{1, 1, 0});
    mesh.iBuffer[0].emplace_back(Indice{2, 2, 0});
    mesh.iBuffer[0].emplace_back(Indice{2, 2, 0});
    mesh.iBuffer[0].emplace_back(Indice{1, 1, 0});
    mesh.iBuffer[0].emplace_back(Indice{3, 3, 0});

    // Face 2
    mesh.iBuffer[0].emplace_back(Indice{2, 0, 1});
    mesh.iBuffer[0].emplace_back(Indice{3, 1, 1});
    mesh.iBuffer[0].emplace_back(Indice{4, 2, 1});
    mesh.iBuffer[0].emplace_back(Indice{4, 2, 1});
    mesh.iBuffer[0].emplace_back(Indice{3, 1, 1});
    mesh.iBuffer[0].emplace_back(Indice{5, 3, 1});

    // Face 3
    mesh.iBuffer[0].emplace_back(Indice{4, 3, 2});
    mesh.iBuffer[0].emplace_back(Indice{5, 2, 2});
    mesh.iBuffer[0].emplace_back(Indice{6, 1, 2});
    mesh.iBuffer[0].emplace_back(Indice{6, 1, 2});
    mesh.iBuffer[0].emplace_back(Indice{5, 2, 2});
    mesh.iBuffer[0].emplace_back(Indice{7, 0, 2});

    // Face 4
    mesh.iBuffer[0].emplace_back(Indice{6, 0, 3});
    mesh.iBuffer[0].emplace_back(Indice{7, 1, 3});
    mesh.iBuffer[0].emplace_back(Indice{0, 2, 3});
    mesh.iBuffer[0].emplace_back(Indice{0, 2, 3});
    mesh.iBuffer[0].emplace_back(Indice{7, 1, 3});
    mesh.iBuffer[0].emplace_back(Indice{1, 3, 3});

    // Face 5
    mesh.iBuffer[0].emplace_back(Indice{1, 0, 4});
    mesh.iBuffer[0].emplace_back(Indice{7, 1, 4});
    mesh.iBuffer[0].emplace_back(Indice{3, 2, 4});
    mesh.iBuffer[0].emplace_back(Indice{3, 2, 4});
    mesh.iBuffer[0].emplace_back(Indice{7, 1, 4});
    mesh.iBuffer[0].emplace_back(Indice{5, 3, 4});

    // Face 6
    mesh.iBuffer[0].emplace_back(Indice{6, 0, 5});
    mesh.iBuffer[0].emplace_back(Indice{0, 1, 5});
    mesh.iBuffer[0].emplace_back(Indice{4, 2, 5});
    mesh.iBuffer[0].emplace_back(Indice{4, 2, 5});
    mesh.iBuffer[0].emplace_back(Indice{0, 1, 5});
    mesh.iBuffer[0].emplace_back(Indice{2, 3, 5});

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

    return mesh;
}

Mesh::CreateArg Mesh::createSphere(int latitudeCount, int longitudeCount)
{
    GPE_ASSERT(latitudeCount > 2 && longitudeCount > 2, "Latitude and Longitude must be greater than 2");

    Mesh::CreateArg mesh;
    mesh.objName = "Sphere";
    mesh.iBuffer.push_back({});

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
                    mesh.iBuffer[0].push_back(Indice{ver1, ver1, ver1});
                    mesh.iBuffer[0].push_back(Indice{ver2, ver2, ver2});
                    mesh.iBuffer[0].push_back(Indice{ver1 + 1, ver1 + 1, ver1 + 1});
                }

                if (i != static_cast<unsigned int>(longitudeCount) - 1)
                {
                    // ver1+1 -> ver2 -> ver2+1 : like this shema
                    //			ver1+1
                    //		 /	  |
                    // ver2	 ->	ver2+1

                    mesh.iBuffer[0].push_back(Indice{ver1 + 1, ver1 + 1, ver1 + 1});
                    mesh.iBuffer[0].push_back(Indice{ver2, ver2, ver2});
                    mesh.iBuffer[0].push_back(Indice{ver2 + 1, ver2 + 1, ver2 + 1});
                }
            }
        }
    }

    return mesh;
}

Mesh::CreateArg Mesh::createCylindre(unsigned int prescision)
{
    GPE_ASSERT(prescision > 2, "Prescision must be greater than 2");

    Mesh::CreateArg mesh;
    mesh.objName = "Cylindre";
    mesh.iBuffer.push_back({});

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
        mesh.iBuffer[0].push_back(Indice{0, 0, 0});
        mesh.iBuffer[0].push_back(Indice{i, i, i});
        mesh.iBuffer[0].push_back(Indice{i + 1, i + 1, i + 1});
    }

    mesh.iBuffer[0].push_back(Indice{0, 0, 0});
    mesh.iBuffer[0].push_back(Indice{prescision, prescision, prescision});
    mesh.iBuffer[0].push_back(Indice{1, 1, 1});

    // side face triangle indice
    for (unsigned int i = 1; i < prescision; i++)
    {
        // face is blit in 2 triangle :
        mesh.iBuffer[0].push_back(Indice{i, i, i});
        mesh.iBuffer[0].push_back(Indice{i + 1, i + 1, i + 1});
        mesh.iBuffer[0].push_back(Indice{i + prescision, i + prescision, i + prescision});

        // triangle 2 :
        mesh.iBuffer[0].push_back(Indice{i + prescision, i + prescision, i + prescision});
        mesh.iBuffer[0].push_back(Indice{i + prescision + 1, i + prescision + 1, i + prescision + 1});
        mesh.iBuffer[0].push_back(Indice{i + 1, i + 1, i + 1});
    }

    // face is blit in 2 triangle :
    mesh.iBuffer[0].push_back(Indice{prescision, prescision, prescision});
    mesh.iBuffer[0].push_back(Indice{1, 1, 1});
    mesh.iBuffer[0].push_back(Indice{prescision + prescision, prescision + prescision, prescision + prescision});

    // triangle 2 :
    mesh.iBuffer[0].push_back(Indice{prescision + prescision, prescision + prescision, prescision + prescision});
    mesh.iBuffer[0].push_back(Indice{prescision + 1, prescision + 1, prescision + 1});
    mesh.iBuffer[0].push_back(Indice{1, 1, 1});

    unsigned int middleFarPointIndice = prescision * 2 + 1;

    // far face triangle indice
    for (unsigned int i = prescision + 1; i < prescision + prescision; i++)
    {
        mesh.iBuffer[0].push_back(Indice{middleFarPointIndice, middleFarPointIndice, middleFarPointIndice});
        mesh.iBuffer[0].push_back(Indice{i, i, i});
        mesh.iBuffer[0].push_back(Indice{i + 1, i + 1, i + 1});
    }

    mesh.iBuffer[0].push_back(Indice{middleFarPointIndice, middleFarPointIndice, middleFarPointIndice});
    mesh.iBuffer[0].push_back(Indice{prescision + prescision, prescision + prescision, prescision + prescision});
    mesh.iBuffer[0].push_back(Indice{prescision + 1, prescision + 1, prescision + 1});

    return mesh;
}

void Mesh::generateNormalAndLoadIndice(const std::vector<Shape>& shapes) noexcept
{
    unsigned int count = 0;
    for (auto&& shape : shapes)
    {
        count++;
        m_iBuffer.push_back({});
        for (size_t i = 0; i < shape.iv.size(); i++)
        {
            m_iBuffer.back().push_back({shape.iv[i], shape.ivt[i], count});
        }

        if (!shape.material_ids.empty())
            m_idMaterial.push_back(shape.material_ids);
    }

    for (auto&& i : m_iBuffer)
    {
        for (size_t i2 = 0; i2 < i.size(); i2 += 3)
        {
            Vec3 v1 = m_vBuffer[i[i2 + 0].iv];
            Vec3 v2 = m_vBuffer[i[i2 + 1].iv];
            Vec3 v3 = m_vBuffer[i[i2 + 2].iv];

            // comput normal with cross product
            m_vnBuffer.emplace_back((v2 - v1).cross(v3 - v1).normalized());
            m_vnBuffer.emplace_back((v1 - v2).cross(v3 - v2).normalized());
            m_vnBuffer.emplace_back((v1 - v3).cross(v2 - v3).normalized());
        }
    }
}