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

    const Model::CreateArg& treeModelArg = *rm.get<Model::CreateArg>("TreeModel");
    GameObject&             forest       = parent.addChild(forestArg);

    // Create trees with random sizes,
    // positions and rotations and add them to the forest
    for (unsigned int i = 0u; i < number; ++i)
    {
        forestArg.name                         = "Tree" + std::to_string(i);
        forestArg.transformArg.position.x      = Random::ranged<float>(-100.f, 100.f);
        forestArg.transformArg.position.z      = Random::ranged<float>(-100.f, 100.f);
        forestArg.transformArg.eulerRotation.y = Random::ranged<float>(TWO_PI);
        forestArg.transformArg.scale           = {Random::ranged<float>(4.f, 8.f)};

        forest.addChild(forestArg).addComponent<Model>(treeModelArg);
    }
}