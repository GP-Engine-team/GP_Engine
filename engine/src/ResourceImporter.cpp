#include "Engine/Resources/ResourceImporter.hpp"

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ObjParser.hpp"
#include "Engine/Core/Tools/Hash.hpp"
#include "Engine/Engine.hpp"
#include <filesystem>

using namespace GPE;

void GPE::importeResource(const char* path) noexcept
{
    std::filesystem::path fsPath(path);
    switch (hash(fsPath.extension().string().c_str()))
    {
    case hash(".obj"):
        Engine::getInstance()->resourceManager.add<Model::CreateArg>(fsPath.filename().string(),
                                                                     importeSingleModel(path));
        break;

    default:
        Log::getInstance()->logWarning(
            stringFormat("Missing implementation for load extention &s", fsPath.extension().string().c_str()));
        break;
    }
}