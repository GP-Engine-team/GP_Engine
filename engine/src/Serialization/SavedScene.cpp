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
    type = loadedType;

    if (type == EType::XML)
    {
        XmlData xmlInfo;
        xmlInfo.data = std::make_unique<char[]>(loadedSize + 1);
        memcpy(xmlInfo.data.get(), buffer, loadedSize);
        xmlInfo.data[loadedSize] = '\0';
        xmlInfo.doc->parse<0>(xmlInfo.data.get());

        info = std::move(xmlInfo);
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