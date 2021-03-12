#include "SingletonsSync.hpp"

extern "C" void setGameSystemsManagerInstance(GPE::SystemsManager& systemManager)
{
	GPE::SystemsManager::setInstance(systemManager);
}