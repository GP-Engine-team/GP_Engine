#pragma once

#include <GPM/Vector3.hpp>
#include <GPM/Quaternion.hpp>
#include <GPM/Matrix4.hpp>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

namespace GPE
{
inline GPM::Vec3 toVec(const aiVector3D& v)
{
    return GPM::Vec3{v.x, v.y, v.z};
}

inline GPM::Quaternion toQuat(const aiQuaternion& aiQuat)
{
    GPM::Quat q;
    q.x = aiQuat.x;
    q.y = aiQuat.y;
    q.z = aiQuat.z;
    q.w = aiQuat.w;
    return q;
}

inline GPM::Matrix4 toMat4(const aiMatrix4x4& aiMat)
{
    GPM::Matrix4 m;
    //for (int i = 0; i < 4; i++)
    //{
    //    for (int j = 0; j < 4; j++)
    //    {
    //        m.e[i * 4 + j] = aiMat[i][j];
    //    }
    //}
    m.e[0*4+0] = aiMat.a1;
    m.e[1*4+0] = aiMat.a2;
    m.e[2*4+0] = aiMat.a3;
    m.e[3*4+0] = aiMat.a4;
    m.e[0*4+1] = aiMat.b1;
    m.e[1*4+1] = aiMat.b2;
    m.e[2*4+1] = aiMat.b3;
    m.e[3*4+1] = aiMat.b4;
    m.e[0*4+2] = aiMat.c1;
    m.e[1*4+2] = aiMat.c2;
    m.e[2*4+2] = aiMat.c3;
    m.e[3*4+2] = aiMat.c4;
    m.e[0*4+3] = aiMat.d1;
    m.e[1*4+3] = aiMat.d2;
    m.e[2*4+3] = aiMat.d3;
    m.e[3*4+3] = aiMat.d4;

    return m;
}

}