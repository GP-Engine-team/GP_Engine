#include "Engine/Resources/Importer/ResourceImporter.hpp"

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Tools/Hash.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Resources/Importer/Importer.hpp"
#include <filesystem>

using namespace GPE;

void GPE::importeResource(const char* src, const char* dst) noexcept
{
    std::filesystem::path srcPath(src);
    std::filesystem::path dstPath(src);
    switch (hash(srcPath.extension().string().c_str()))
    {

    case hash(".obj"):
    case hash(".fbx"):
        importeModel(src, dst);
        break;

    case hash(".jpg"):
    case hash(".jpeg"):
    case hash(".png"):
    case hash(".bmp"):
    case hash(".tga"):

        importeTextureFile(src, dst);
        break;

    default:
        Log::getInstance()->logWarning(stringFormat("Missing implementation to importe file \"%s\"", src));
        break;
    }
}