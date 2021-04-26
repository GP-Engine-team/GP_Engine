#pragma once

#include <rapidxml/rapidxml.hpp>
#include <memory>

namespace GPE
{

class SaveFile
{
    struct XmlData
    {
        rapidxml::xml_document<> doc;

    };

    enum class EType
    {
        BINARY,
        XML,
        NONE,
    };

    union Data
    {
        XmlData xmlData;
    } data;

    EType type = EType::NONE;

    void loadFromMemory(const unsigned char* data, size_t loadedSize, EType loadedType);

    std::unique_ptr<std::string> saveToMemory(size_t& savedSize, EType& savedType);
};

} // namespace GPE