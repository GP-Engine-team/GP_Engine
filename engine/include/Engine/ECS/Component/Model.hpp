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

// Generated
#include "Generated/Model.rfk.h"

namespace GPE RFKNamespace()
{

    class Mesh;
    class Shader;
    class Material;
    class Model;
    struct KeyFrame;

    struct SubModel;

    struct SubModel
    {
        struct CreateArg
        {
            Shader*   pShader;
            Material* pMaterial;
            Mesh*     pMesh;

            bool enableBackFaceCulling = true;
            bool castShadow            = true;
        };

        SubModel(Model& model, const CreateArg& arg)
            : pModel{&model}, pShader{arg.pShader}, pMaterial{arg.pMaterial}, pMesh{arg.pMesh},
              enableBackFaceCulling{arg.enableBackFaceCulling}, castShadow{arg.castShadow}
        {
        }

        SubModel() = default;

        bool isValide()
        {
            return pModel && pMesh && pShader && pMaterial;
        }

        Model*    pModel    = nullptr;
        Shader*   pShader   = nullptr;
        Material* pMaterial = nullptr;
        Mesh*     pMesh     = nullptr;

        bool enableBackFaceCulling = true;
        bool castShadow            = true;
    };

    template <>
    void save(XmlSaver & context, const SubModel& inspected, const XmlSaver::SaveInfo& info);
    template <>
    void load(XmlLoader & context, SubModel & inspected, const XmlLoader::LoadInfo& info);

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
        struct AnimRenderFrame* m_animBufferData = nullptr;

    public:
        Model()        = default;
        Model(GameObject & owner);
        Model(GameObject & owner, const CreateArg& arg);

        virtual ~Model();

        void moveTowardScene(class Scene & newOwner) override;

        virtual void inspect(InspectContext & context);

        void addSubModel(const SubModel::CreateArg& arg);

        Model_GENERATED
    };
} // namespace )
