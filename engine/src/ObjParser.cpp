#include "Engine/Core/Parsers/ObjParser.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ParserTool.hpp"

using namespace GPE;
using namespace GPM;

inline void parseVertexPos(const std::string& line, std::vector<Vec3>& vBuffer)
{
    std::size_t cursorStart = line.find_first_not_of(" ", 1);
    std::size_t cursorEnd   = line.find(" ", cursorStart);
    float       x           = std::stof(line.substr(cursorStart, cursorEnd - cursorStart));

    cursorStart = line.find_first_not_of(" ", cursorEnd);
    cursorEnd   = line.find(" ", cursorStart);
    float y     = std::stof(line.substr(cursorStart, cursorEnd - cursorStart));

    cursorStart = line.find_first_not_of(" ", cursorEnd);
    cursorEnd   = line.find(" ", cursorStart);
    float z     = std::stof(line.substr(cursorStart, cursorEnd - cursorStart));

    vBuffer.push_back({x, y, z});
}

inline void parseTexturePos(const std::string& line, std::vector<Vec2>& vtBuffer)
{
    std::size_t cursorStart = line.find_first_not_of(" ", 2);
    std::size_t cursorEnd   = line.find(" ", cursorStart);
    float       tx          = std::stof(line.substr(cursorStart, cursorEnd - cursorStart));

    cursorStart = line.find_first_not_of(" ", cursorEnd);
    cursorEnd   = line.find(" ", cursorStart);
    float ty    = std::stof(line.substr(cursorStart, cursorEnd - cursorStart));

    vtBuffer.push_back({tx, ty});
}

inline void parseNormal(const std::string& line, std::vector<Vec3>& vnBuffer)
{
    std::size_t cursorStart = line.find_first_not_of(" ", 2);
    std::size_t cursorEnd   = line.find(" ", cursorStart);
    float       x           = std::stof(line.substr(cursorStart, cursorEnd - cursorStart));

    cursorStart = line.find_first_not_of(" ", cursorEnd);
    cursorEnd   = line.find(" ", cursorStart);
    float y     = std::stof(line.substr(cursorStart, cursorEnd - cursorStart));

    cursorStart = line.find_first_not_of(" ", cursorEnd);
    cursorEnd   = line.find(" ", cursorStart);
    float z     = std::stof(line.substr(cursorStart, cursorEnd - cursorStart));

    vnBuffer.push_back({x, y, z});
}

inline unsigned int getNumberIndice(const std::string& line, unsigned int numberAttributeByIndice)
{
    return numberAttributeByIndice == 1 ? 0 : static_cast<unsigned int>(std::count(line.begin(), line.end(), '/')) / (numberAttributeByIndice - 1);
}

inline unsigned int getNumberAttributeByIndice(const std::string& line)
{
    std::size_t cursorStart = line.find_first_not_of(" ", 1);
    std::string indiceBlock = line.substr(cursorStart, line.find_first_of(" ", cursorStart) - cursorStart);

    return static_cast<unsigned int>(std::count(indiceBlock.begin(), indiceBlock.end(), '/')) + 1;
}

inline void parseIndice(const std::string& line, std::vector<Shape>& shape)
{
    unsigned int firstIv(0), firstIvn(0), firstIvt(0), secondIv(0), secondIvn(0), secondIvt(0);
    std::size_t  cursorStart, cursorEnd;
    unsigned int numberAttributeByIndice = getNumberAttributeByIndice(line);
    unsigned int numberIndice            = getNumberIndice(line, numberAttributeByIndice);

    if (numberIndice == 0)
    {
        Log::logWarning((std::string("Indice without vn not accept. Line not read : ") + line).c_str());
        return;
    }

    if (shape.empty())
    {
        shape.push_back({});
    }

    // get indices of thirst vertex
    cursorStart = line.find_first_not_of(" ", 1);

    cursorEnd = line.find('/');
    firstIv   = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;

    if (numberAttributeByIndice > 1)
    {
        cursorStart = cursorEnd + 1;

        if (numberAttributeByIndice > 2)
        {
            cursorEnd   = line.find('/', cursorStart);
            firstIvt    = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;
            cursorStart = cursorEnd + 1;
            cursorEnd   = line.find(' ', cursorStart);
            firstIvn    = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;
        }
        else
        {
            cursorEnd = line.find(' ', cursorStart);
            firstIvt  = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;
        }
    }

    // Parse the second index and save it indice
    cursorStart = cursorEnd + 1;
    cursorEnd   = line.find('/', cursorStart);
    secondIv    = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;

    if (numberAttributeByIndice > 1)
    {
        cursorStart = cursorEnd + 1;

        if (numberAttributeByIndice > 2)
        {
            cursorEnd   = line.find('/', cursorStart);
            secondIvt   = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;
            cursorStart = cursorEnd + 1;
            cursorEnd   = line.find(' ', cursorStart);
            secondIvn   = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;
        }
        else
        {
            cursorEnd = line.find(' ', cursorStart);
            secondIvt = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;
        }
    }

    // Parse all the indices
    for (size_t i = 0; i < numberIndice - 2; i++)
    {
        shape.back().iv.push_back(firstIv);
        shape.back().ivn.push_back(firstIvn);
        shape.back().ivt.push_back(firstIvt);

        shape.back().iv.push_back(secondIv);
        shape.back().ivn.push_back(secondIvn);
        shape.back().ivt.push_back(secondIvt);

        // get indice of second vertex
        cursorStart = cursorEnd + 1;
        cursorEnd   = line.find('/', cursorStart);
        secondIv    = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;

        if (numberAttributeByIndice > 1)
        {
            cursorStart = cursorEnd + 1;

            if (numberAttributeByIndice > 2)
            {
                cursorEnd   = line.find('/', cursorStart);
                secondIvt   = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;
                cursorStart = cursorEnd + 1;
                cursorEnd   = line.find(' ', cursorStart);
                secondIvn   = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;
            }
            else
            {
                cursorEnd = line.find(' ', cursorStart);
                secondIvt = std::atoi(line.substr(cursorStart, cursorEnd - cursorStart).c_str()) - 1;
            }
        }

        shape.back().iv.push_back(secondIv);
        shape.back().ivn.push_back(secondIvn);
        shape.back().ivt.push_back(secondIvt);
    }
}

inline void parseMtl(const std::string& line, std::vector<MaterialAttrib>& materials)
{
    std::string path;
    path += PATH_MTL + line.substr(7, line.find_last_of('.') - 7) + EXTENSION_MTL;

    loadMTL(path, materials);
}

inline void parseUseMtl(const std::string& line, std::vector<Shape>& shape)
{
    shape.push_back({});
    parseName(line, shape.back().material_ids);
}

inline void parseSurfaceName(const std::string& line, std::vector<Shape>& shape)
{
    if (!shape.empty())
        parseName(line, shape.back().name);
}

inline void parseLine(const std::string& line, Attrib* attrib, std::vector<Shape>* shape,
                      std::vector<MaterialAttrib>* materials)
{
    // Detect line type and parse this line
    if (isTitle(line, "#"))
        return;
    else if (isTitle(line, "o") && attrib != nullptr)
    {
        parseName(line, attrib->objName);
        Log::log((std::string("Obj name : ") + attrib->objName).c_str());
    }
    else if (isTitle(line, "g") && shape != nullptr)
        parseSurfaceName(line, *shape);
    else if (isTitle(line, "mtllib") && materials != nullptr)
        parseMtl(line, *materials);
    else if (isTitle(line, "usemtl") && shape != nullptr)
        parseUseMtl(line, *shape);
    else if (isTitle(line, "vt") && attrib != nullptr)
        parseTexturePos(line, attrib->vtBuffer);
    else if (isTitle(line, "vn") && attrib != nullptr)
        parseNormal(line, attrib->vnBuffer);
    else if (isTitle(line, "v") && attrib != nullptr)
        parseVertexPos(line, attrib->vBuffer);
    else if (isTitle(line, "f") && shape != nullptr)
        parseIndice(line, *shape);
    else
        Log::logWarning(std::string("Obj Parser don't parse this line : " + line).c_str());
}

void GPE::loadObjWithMTL(const char* objPath, Attrib* attrib, std::vector<Shape>* shape,
                           std::vector<MaterialAttrib>* materials) noexcept
{
    GPE_ASSERT(objPath != nullptr, "Void path");

    Log::logInitializationStart("Obj parsing");

    std::ifstream file(objPath);

    if (!file)
    {
        Log::logError((std::string("Error to found or read file : ") + objPath).c_str());
        exit(1);
    }

    std::string line;
    while (getline(file >> std::ws, line))
    {
        if (line.size() < 2)
            continue;

        parseLine(line, attrib, shape, materials);
    }
    Log::logInitializationEnd("Obj parsing");
}