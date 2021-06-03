/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <GPM/Matrix4.hpp>
#include <vector>

// Generated
#include <Generated/AnimationComponent.rfk.h>

namespace GPE RFKNamespace()
{
class Skin;
class Animation;

class RFKClass(Inspect(), ComponentGen(), Serialize()) AnimationComponent : public Component
{
private:
    RFKField(Inspect("setSkeleton"), Serialize()) Skeleton*  m_skeleton         = nullptr;
    RFKField(Inspect(), Serialize()) Animation*              m_currentAnimation = nullptr;
    //RFKField() Animation*                                    m_nextAnimation    = nullptr;
    class Model*                                             m_model            = nullptr;
    RFKField(Inspect("setSkin"), Serialize()) Skin*          m_skin             = nullptr;
    RFKField(Inspect(), Serialize()) float                   m_currentTime = 0.f; // in seconds
    //RFKField(Inspect(), Serialize()) float                   m_nextAnimTime = 0.f;
    RFKField(Inspect(), Serialize()) float                   m_timeScale = 1.f;

    //float blendAlpha = 0.f;

public:
    std::vector<GPM::Mat4> m_finalBoneMatrices;
    bool                   shouldLoop = true;

public:
    AnimationComponent(class GameObject & owner) noexcept;
    AnimationComponent() noexcept = default;
    virtual ~AnimationComponent() noexcept;

protected:
    void         removeAnimData();
    void         updateAnimData(bool wasComplete);
    virtual void updateToSystem() noexcept override;

public:
    /**
        * @brief Update the animation.
        * @param dt
        */
    void update(float dt);

    void playAnimation(Animation * pAnimation);

    void calculateBoneTransform(const struct AssimpNodeData& node, const GPM::mat4& parentTransform);

    void setModel(Model* model);
    void setSkeleton(Skeleton* skeleton);
    void setSkin(Skin* skin);

    void drawDebugSkeleton(const GPM::Vec4& offset = GPM::Vec4{0, -100, 0}) const;

    virtual void onPostLoad() override;

    bool isComplete() const;

    GETTER_BY_VALUE(TimeScale, m_timeScale);
    GETTER_BY_VALUE(CurrentTime, m_currentTime);

    AnimationComponent_GENERATED
};
} // namespace )