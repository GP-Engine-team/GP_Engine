#include "Engine/Core/Debug/Log.hpp"
#include "GameStartup.hpp"

#include "Refureku/Refureku.h"

using namespace GPE;

#ifdef _WIN32
// Use discrete GPU by default.
extern "C"
{
	// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	__declspec(dllexport) int NvOptimusEnablement = 1;

	// https://community.amd.com/t5/firepro-development/can-an-opengl-app-default-to-the-discrete-gpu-on-an-enduro/m-p/279441#M471
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

int main(int argc, char* argv[])
{
	auto a = rfk::Database::getNamespace("GPE");
	//std::cout << a->getClass("BehaviourComponent")->getField("speed") << std::endl;

	//Log::getInstance()->logInitializationStart("GameStartup creation");

	//GameStartup gameStartup;
	//gameStartup.run();

	return 0;
}
