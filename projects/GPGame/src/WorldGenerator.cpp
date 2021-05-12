#include <WorldGenerator.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Resources/ResourcesManagerType.hpp>
#include <GPM/Random.hpp>

#include "Generated/WorldGenerator.rfk.h"
File_GENERATED

    using namespace GPG;
using namespace GPM;
using namespace GPE;

void WorldGenerator::loadTree(GameObject& parent, unsigned int number)
{
    const ResourceManagerType& rm = Engine::getInstance()->resourceManager;
    GameObject::CreateArg      forestArg{"Forest"};

    // Create trees with random sizes,
    // positions and rotations and add them to the forest
    for (unsigned int i = 0u; i < number; ++i)
    {
        GameObject* const newGO = m_treePrefab.clone(*m_treeContainer);

        newGO->setName(("Tree" + std::to_string(i)).c_str());
        Vec3 pos;
        pos.x = Random::ranged<float>(-100.f, 100.f);
        pos.y = newGO->getTransform().getGlobalPosition().y;
        pos.z = Random::ranged<float>(-100.f, 100.f);
        newGO->getTransform().setTranslation(pos);

        Vec3 rotEuler = newGO->getTransform().getRotation().eulerAngles();
        rotEuler.y    = Random::ranged<float>(TWO_PI);
        newGO->getTransform().setRotation(Quat::fromEuler(rotEuler));

        newGO->getTransform().setScale({Random::ranged<float>(4.f, 8.f)});
    }
}