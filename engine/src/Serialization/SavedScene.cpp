#include "Engine/Serialization/SavedScene.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <sstream>

namespace GPE
{
void SavedScene::loadFromMemory(const char* buffer, size_t loadedSize, EType loadedType)
{
    if (type == EType::XML && std::get<XmlData>(info).rawData != nullptr)
        delete std::get<XmlData>(info).rawData;

    type = loadedType;

    if (type == EType::XML)
    {
        XmlData xmlInfo;
        xmlInfo.rawData             = new char[loadedSize + 1];
        xmlInfo.rawData[loadedSize] = '\0';
        memcpy(xmlInfo.rawData, buffer, loadedSize);
        xmlInfo.doc->parse<0>(xmlInfo.rawData);

        info = std::variant<XmlData> (std::move(xmlInfo));
    }
}

std::unique_ptr<char[]> SavedScene::saveToMemory(size_t& savedSize, EType& savedType)
{
    savedType = type;

    std::string str;
    std::unique_ptr<char[]> cbuffer;

    switch (type)
    {
    case EType::XML:
        XmlData&          xmlInfo = std::get<XmlData>(info);
        std::stringstream ss;
        ss << xmlInfo.doc;
        ss >> str;
        cbuffer = std::make_unique<char[]>(str.size() + 1);
        memcpy(cbuffer.get(), str.c_str(), str.size() + 1);
        break;
    }

    return cbuffer;
}

} // namespace GPE