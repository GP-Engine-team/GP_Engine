#include "Engine/Core/Parsers/MTLParser.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ParserTool.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace GPE;
using namespace GPM;

enum class EComponentType
{
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    EMISSION
};

static void parseComponent(std::string& line, std::vector<MaterialAttrib>& materials, EComponentType type)
{
    std::size_t cursorStart = line.find_first_not_of(" ", 2);
    std::size_t cursorEnd   = line.find(" ", cursorStart);
    float       x           = parse<float>(line, cursorStart, cursorEnd - cursorStart);

    cursorStart = line.find_first_not_of(" ", cursorEnd);
    cursorEnd   = line.find(" ", cursorStart);
    float y     = parse<float>(line, cursorStart, cursorEnd - cursorStart);

    cursorStart = line.find_first_not_of(" ", cursorEnd);
    cursorEnd   = line.find(" ", cursorStart);
    float z     = parse<float>(line, cursorStart, cursorEnd - cursorStart);

    parse<float>(line, 2);

    switch (type)
    {
    case EComponentType::AMBIENT:
        materials.back().ambient.rgbi = {x, y, z, 1.f};
        break;

    case EComponentType::DIFFUSE:
        materials.back().diffuse.rgbi = {x, y, z, 1.f};
        break;

    case EComponentType::SPECULAR:
        materials.back().specular.rgbi = {x, y, z, 1.f};
        break;
    default:
        break;
    }
}

static void parseTextureName(std::string& line, std::vector<MaterialAttrib>& materials, EComponentType type)
{
    switch (type)
    {
    case EComponentType::AMBIENT:
        parseName(line, materials.back().nameAmbiantTexture);
        break;

    case EComponentType::DIFFUSE:
        parseName(line, materials.back().nameDiffuseTexture);
        break;

    case EComponentType::SPECULAR:
        parseName(line, materials.back().nameSpecularTexture);
        break;
    default:
        break;
    }
}

inline void createNewMaterialAndParseName(std::string line, std::vector<MaterialAttrib>& materials)
{
    materials.push_back({});
    parseName(line, materials.back().id);
}

inline void parseLine(std::string line, std::vector<MaterialAttrib>& materials)
{
    // Detect line type and parse this line
    if (isTitle(line, "#"))
        return;
    else if (isTitle(line, "newmtl"))
        createNewMaterialAndParseName(line, materials);
    else if (isTitle(line, "Ka"))
        parseComponent(line, materials, EComponentType::AMBIENT);
    else if (isTitle(line, "Kd"))
        parseComponent(line, materials, EComponentType::DIFFUSE);
    else if (isTitle(line, "Ks"))
        parseComponent(line, materials, EComponentType::SPECULAR);
    else if (isTitle(line, "Ns"))
        materials.back().shininess = parse<float>(line, 2);
    else if (isTitle(line, "map_Ka"))
        parseTextureName(line, materials, EComponentType::AMBIENT);
    else if (isTitle(line, "map_Kd"))
        parseTextureName(line, materials, EComponentType::DIFFUSE);
    else if (isTitle(line, "map_Ks"))
        parseTextureName(line, materials, EComponentType::SPECULAR);
    else
        Log::logWarning(std::string("MTL Parser cannot read this line : " + line).c_str());
}

void GPE::loadMTL(std::string& materialPath, std::vector<MaterialAttrib>& materials) noexcept
{
    std::ifstream file(materialPath);

    if (!file)
    {
        Log::logError((std::string("Error to found or read file : ") + materialPath).c_str());
        exit(1);
    }

    std::string line;
    while (getline(file >> std::ws, line))
    {
        if (line.size() < 2)
            continue;

        parseLine(line, materials);
    }

    Log::log("Material parsing done");
}