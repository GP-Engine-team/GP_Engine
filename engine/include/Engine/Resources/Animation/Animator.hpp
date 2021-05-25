#pragma once

#include <vector>
#include <Engine/Resources/Animation/Animation.hpp>
#include <GPM/Matrix4.hpp>

namespace GPE
{
class Animator
{
public:
    Animator(Animation* animation)
    {
        m_CurrentTime      = 0.0;
        m_CurrentAnimation = animation;

        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(GPM::Mat4::identity());
    }

    void UpdateAnimation(float dt)
    {
        m_DeltaTime = dt;
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt * 30;
            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), GPM::Mat4::identity());
        }
    }

    void PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime      = 0.0f;
    }

    void CalculateBoneTransform(const AssimpNodeData* node, const GPM::mat4& parentTransform)
    {
        std::string nodeName      = node->name;
        GPM::Mat4   nodeTransform = node->transformation;

        Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

        if (Bone)
        {
            Bone->Update(m_CurrentTime);
            nodeTransform = Bone->GetLocalTransform();
        }

        GPM::Mat4 globalTransformation = parentTransform * nodeTransform;

        auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int       index            = boneInfoMap[nodeName].id;
            GPM::Mat4 offset           = boneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
            if (std::isnan(m_FinalBoneMatrices[index].e[0]))
            {
                int i;
                i = 5;
            }
        }

        for (int i = 0; i < node->childrenCount; i++)
            CalculateBoneTransform(&node->children[i], globalTransformation);
    }

    std::vector<GPM::Mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

private:
    std::vector<GPM::Mat4> m_FinalBoneMatrices;
    Animation*             m_CurrentAnimation;
    float                  m_CurrentTime;
    float                  m_DeltaTime;
};
}