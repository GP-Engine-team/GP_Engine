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
    RFKField(Inspect("setSkeleton"), Serialize()) Skeleton*    m_skeleton         = nullptr;
    RFKField(Inspect("playAnimation"), Serialize()) Animation* m_currentAnimation = nullptr;
    class Model*                                               m_model            = nullptr;
    RFKField(Inspect("setSkin"), Serialize()) Skin*            m_skin             = nullptr;
    RFKField(Inspect(), Serialize()) float                     m_currentTime = 0.f; // in seconds
    //RFKField(Inspect(), Serialize()) float                   m_nextAnimTime = 0.f;
    RFKField(Inspect()) float                     m_timeScale = 1.f;
    RFKField(Inspect("setSubModelIndex"), Serialize()) int     m_subModelIndex = -1;

    RFKField(Inspect(), Serialize()) float                     m_nextAnimTime     = 0.f; // in seconds
    RFKField() Animation*                                      m_nextAnimation = nullptr;
    RFKField(Inspect()) float                                  m_nextTimeScale    = 1.f;
    RFKField() float                                           m_blendTime        = 0.3f;

    //float blendAlpha = 0.f;
    std::vector<size_t> skeletonBoneIDToAnimationBoneID;
    std::vector<size_t> skeletonBoneIDToNextAnimBoneID;

public:
    std::vector<GPM::Mat4>                m_finalBoneMatrices;
    RFKField(Inspect(), Serialize()) bool shouldLoop = true;
    RFKField(Inspect(), Serialize()) bool shouldNextLoop = true;

public:
    AnimationComponent(class GameObject & owner) noexcept;
    AnimationComponent() noexcept = default;
    virtual ~AnimationComponent() noexcept;

private:
    void setSubModelIndex(int newSubModelIndex);
    void setNextAnimAsCurrent();

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

    void playAnimation(Animation * pAnimation, float startTime = 0.f, float nextTimeScale = 1.f);
    void setCurrentTime(float newTime);

    void calculateBoneTransform(const struct AssimpNodeData& node, const GPM::mat4& parentTransform);

    void setModel(Model* model);
    void setSkeleton(Skeleton* skeleton);
    void setSkin(Skin* skin);
    void setCurrentAnimDuration(float newDuration);
    void setNextAnim(Animation* nextAnim, float blendTime = 1.f, float nextTimeScale = 1.f);

    virtual void onPostLoad() override;

    bool isComplete() const;

    GETTER_BY_VALUE(TimeScale, m_timeScale);
    GETTER_BY_VALUE(CurrentTime, m_currentTime);
    GETTER_BY_VALUE(CurrentAnimation, m_currentAnimation);

    AnimationComponent_GENERATED
};
} // namespace )