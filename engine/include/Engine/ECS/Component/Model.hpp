/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <memory> //std::shared_ptr
#include <vector> //std::vector

#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Serialization/ComponentGen.h"
#include "GPM/Shape3D/Volume.hpp"

// Generated
#include "Generated/Model.rfk.h"

namespace GPE RFKNamespace()
{

    class Mesh;
    class Shader;
    class Material;
    class Model;

    struct SubModel
    {
        Model*    pModel    = nullptr;
        Shader*   pShader   = nullptr;
        Material* pMaterial = nullptr;
        Mesh*     pMesh     = nullptr;

        bool enableBackFaceCulling = true;
    };

    template <>
    void DataInspector::inspect(GPE::InspectContext & context, SubModel & inspected);

    bool isSubModelHasPriorityOverAnother(const SubModel* lhs, const SubModel* rhs) noexcept;

    class RFKClass(ComponentGen) Model : public Component
    {
    public:
        struct CreateArg
        {
            std::list<SubModel> subModels;
        };

    protected:
        RFKField(Inspect()) std::list<SubModel> m_subModels;

    public:
        Model(GameObject & owner);

        Model(GameObject & owner, const CreateArg& arg);

        Model(const Model& other) noexcept = delete;
        Model(Model && other) noexcept;
        virtual ~Model();

        Model()        = default;
        Model& operator=(Model const& other) = delete;
        Model& operator                      =(Model&& other) noexcept;

        void moveTowardScene(class Scene & newOwner) override;

        virtual void inspect(InspectContext & context);

        /**
         * @brief Add or remove current component from it's system which have for effect to enable or disable it
         * @param newState
         * @return
         */
        void setActive(bool newState) noexcept override;

        Model_GENERATED
    };
} // namespace )
