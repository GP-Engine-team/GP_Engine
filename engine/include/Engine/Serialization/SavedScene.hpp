#pragma once

#include <memory>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <variant>

namespace GPE
{

class SavedScene
{
    struct XmlData
    {
        std::unique_ptr<rapidxml::xml_document<>> doc;
        char*                    rawData = nullptr;

        XmlData() = default;
        XmlData& operator=(XmlData&& rhs)
        {
            doc         = std::move(rhs.doc);
            rawData     = rhs.rawData;
            rhs.rawData = nullptr;
        }
        XmlData& operator=(const XmlData& rhs) = delete;
        ~XmlData()
        {
            if (rawData != nullptr)
                delete rawData;
        }
    };

    enum class EType
    {
        BINARY,
        XML,
        NONE,
    };

    std::variant<XmlData> info;

    EType type = EType::NONE;

public:
    SavedScene()                = default;
    SavedScene(const SavedScene&) = delete;
    SavedScene(SavedScene&&)      = default;

    SavedScene& operator=(SavedScene&&) = default;
    SavedScene& operator=(const SavedScene&) = delete;

    void loadFromMemory(const char* data, size_t loadedSize, EType loadedType);

    std::unique_ptr<char[]> saveToMemory(size_t& savedSize, EType& savedType);
};

} // namespace GPE