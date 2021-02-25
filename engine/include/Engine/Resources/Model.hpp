/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector> //std::vector
#include <memory> //std::shared_ptr

#include "Engine/Intermediate/Component.hpp"
#include "GPM/Shape3D/Volume.hpp"

namespace GPE
{

class Mesh;
class Shader;
class Material;
class Model;

struct SubModel
{
    Model*    pModel   = nullptr;
    Shader*   pShader   = nullptr;
    Material* pMaterial = nullptr;
    Mesh*     pMesh     = nullptr;

    bool enableBackFaceCulling = true;
};

bool isSubModelHasPriorityOverAnother(const SubModel* lhs, const SubModel* rhs) noexcept;

class Model : public Component
{
public:
    struct CreateArg
    {
        std::vector<SubModel>* pSubModels;
    };

protected:
    std::vector<SubModel> m_subModels;

public:
    Model(GameObject& owner, const CreateArg& arg);

    Model(const Model& other) noexcept;
    Model(Model&& other) noexcept;
    virtual ~Model();

    Model()        = delete;
    Model& operator=(Model const& other) = delete;
    Model& operator=(Model&& other) = delete;
};

#include "Model.inl"

} /*namespace GPE*/