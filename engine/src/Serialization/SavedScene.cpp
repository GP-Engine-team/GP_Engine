#include "Engine/Serialization/SavedScene.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <sstream>

namespace GPE
{
SavedScene::SavedScene(const CreateArg& arg)
{
    loadFromMemory(arg.data.c_str(), arg.data.size(), arg.type);
}

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

std::string SavedScene::saveToMemory(EType& savedType)
{
    savedType = type;
    std::string str;

    switch (type)
    {
    case EType::XML: {

        XmlData&          xmlInfo = std::get<XmlData>(info);
        std::stringstream ss;
        ss << xmlInfo.doc;
        ss >> str;
    }
    break;

    default:
        break;
    }

    return str;
}

} // namespace GPE