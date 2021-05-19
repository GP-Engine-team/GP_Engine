#include <Engine/Core/Debug/Assert.hpp>
#include <Engine/Resources/Animation.hpp>

namespace GPE
{
const KeyFrame& Animation::getPreviousKeyFrame(float playTimeRatio) const
{
    float fSize = keyFrames.size();

    float  index          = playTimeRatio * fSize;
    size_t prevFrameIndex = static_cast<size_t>(std::floor(index));

    return keyFrames[prevFrameIndex % keyFrames.size()];
}
const KeyFrame& Animation::getNextKeyFrame(float playTimeRatio) const
{
    float fSize = keyFrames.size();

    float  index          = playTimeRatio * fSize;
    size_t nextFrameIndex = static_cast<size_t>(std::ceil(index));

    return keyFrames[nextFrameIndex % keyFrames.size()];
}
void Animation::getKeyFrame(float playTimeRatio, KeyFrame& buffer) const
{
    float index     = playTimeRatio * keyFrames.size();
    int   prevFrame = static_cast<int>(std::floor(index));

    KeyFrame::blend(getPreviousKeyFrame(playTimeRatio), getNextKeyFrame(playTimeRatio), index - prevFrame, buffer);
}

void Animation::loadAnimFromAssimp(const AssimpAnimationData& animData)
{
    keyFrames.resize(animData.nbFrames);

    for (size_t frameIndex = 0; frameIndex < animData.nbFrames; frameIndex++)
    {
        KeyFrame& frame = keyFrames[frameIndex];

        frame.bones.resize(animData.nbBones);

        // For Each Bone, Store Local Anim Location and Rotation
        for (size_t boneIndex = 0; boneIndex < animData.nbBones; boneIndex++)
        {
            GPM::SplitTransform split = frame.bones[boneIndex];

            auto& assimpBone = animData.frame[frameIndex].bones[boneIndex];

            split.position = assimpBone.position;
            split.rotation = assimpBone.rotation;
        }
    }

    int nbBones = getNbBones();
}

void KeyFrame::draw(float playTimeRatio, Buffer& buffer)
{
    //RenderFrameConstructor renderFrameConstructor = RenderFrameConstructor{skeleton, GetKeyFrame(playTimeRatio)};
    //RenderFrame            renderFrame{std::move(renderFrameConstructor)};

    //buffer.setData();

    //buffer.DrawSkeleton();
}

void KeyFrame::blend(const KeyFrame& key1, const KeyFrame& key2, float alpha, KeyFrame& buffer)
{
    size_t nbBones = key1.getNbBones();
    buffer.bones.resize(nbBones);

    // For Each Bone, Store Blended Anim Location and Rotation
    for (size_t i = 0; i < nbBones; i++)
    {
        /* Interpolates locations */
        buffer.bones[i].position = key1.bones[i].position.lerp(key2.bones[i].position, alpha);
        /* Interpolates rotations */
        buffer.bones[i].rotation = key1.bones[i].rotation.nlerp(key2.bones[i].rotation, alpha);
    }
}

} // namespace GPE