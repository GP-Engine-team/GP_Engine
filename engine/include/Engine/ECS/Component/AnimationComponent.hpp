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
class RFKClass(Inspect(), ComponentGen(), Serialize()) AnimationComponent : public Component
{
private:
    class Skeleton*                        m_skeleton         = nullptr;
    class Animation*                       m_currentAnimation = nullptr;
    class Model*                           m_model            = nullptr;
    class Skin*                            m_skin             = nullptr;
    RFKField(Inspect(), Serialize()) float m_currentTime;
    RFKField(Inspect(), Serialize()) float m_timeScale = 30.f;

    RFKField(Inspect("setupAnims")) bool debugAnimUpdateCallback = false;
    void                                 setupAnims(bool);

public:
    std::vector<GPM::Mat4> m_finalBoneMatrices;

public:
    AnimationComponent(class GameObject & owner) noexcept;
    AnimationComponent() noexcept = default;
    virtual ~AnimationComponent() noexcept;

protected:
    virtual void updateToSystem() noexcept override;

public:
    /**
        * @brief Update the animation.
        * @param dt
        */
    void update(float dt);

    void playAnimation(Animation * pAnimation);

    void calculateBoneTransform(const struct AssimpNodeData* node, const GPM::mat4& parentTransform);

    void setSkeleton(Skeleton* skeleton);

    GETTER_BY_VALUE(TimeScale, m_timeScale);
    GETTER_BY_VALUE(CurrentTime, m_currentTime);

    AnimationComponent_GENERATED
};
} // namespace )