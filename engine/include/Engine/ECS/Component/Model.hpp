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
#include "Engine/Serialization/xml/xmlSaver.hpp"
#include "GPM/Shape3D/Volume.hpp"
#include "GPM/Shape3D/AABB.hpp"

// Generated
#include "Generated/Model.rfk.h"

namespace GPM
{
union Matrix4;
using Mat4 = Matrix4;
}

namespace GPE RFKNamespace()
{
    class Mesh;
    class Shader;
    class Material;
    class Model;
    class Skeleton;
    struct SubModel;

    class AnimationComponent;

    struct RFKStruct(Serialize()) SubModel
    {
        struct CreateArg
        {
            Shader*   pShader;
            Material* pMaterial;
            Mesh*     pMesh;

            bool enableBackFaceCulling = true;
            bool castShadow            = true;
            bool isTransparent         = false;
        };

        SubModel(Model & model, const CreateArg& arg)
            : pModel{&model}, pShader{arg.pShader}, pMaterial{arg.pMaterial}, pMesh{arg.pMesh},
              enableBackFaceCulling{arg.enableBackFaceCulling}, castShadow{arg.castShadow},
              isTransparent{arg.isTransparent}
        {
        }

        SubModel() = default;
        ~SubModel();

        bool isValid() const;
        std::vector<GPM::Mat4>& getFinalBonesTransforms() const;
        bool isAnimated() const;

        RFKField(Serialize()) Model*    pModel    = nullptr;
        RFKField(Serialize()) Shader*   pShader   = nullptr;
        RFKField(Serialize()) Material* pMaterial = nullptr;
        RFKField(Serialize()) Mesh*     pMesh     = nullptr;
        AnimationComponent*             pAnimComponent = nullptr;

        RFKField(Serialize()) bool enableBackFaceCulling = true;
        RFKField(Serialize()) bool castShadow            = true;
        RFKField(Serialize()) bool isTransparent         = false;
        RFKField(Serialize()) bool writeInDepth          = true;

        SubModel_GENERATED
    };

    template <>
    void DataInspector::inspect(GPE::InspectContext & context, SubModel & inspected);

    bool isSubModelHasPriorityOverAnother(const SubModel* lhs, const SubModel* rhs) noexcept;

    class RFKClass(ComponentGen, Serialize()) Model : public Component
    {
    public:
        struct CreateArg
        {
            std::list<SubModel> subModels;
        };

    protected:
        RFKField(Inspect(), Serialize()) std::list<SubModel> m_subModels;

        virtual void updateToSystem() noexcept override;

    public:
        Model() = default;
        Model(GameObject & owner);
        Model(GameObject & owner, const CreateArg& arg);

        virtual ~Model();

        void moveTowardScene(class Scene & newOwner) override;

        virtual void inspect(InspectContext & context);

        void addSubModel(const SubModel::CreateArg& arg);

        void setAnimComponent(AnimationComponent* newAnimComp, int subModelIndex);

        void bindSkin(class Skin& skin, int subModelIndex);

        /**
         * @brief Function to return the local AABB (do not considere the position, scale and rotation of transform)
         * @return
         */
        GPM::AABB getLocalAABB();
        GPM::Vec3 getLocalAABBMin();
        GPM::Vec3 getLocalAABBMAx();

        Model_GENERATED
    };
} // namespace )
