#pragma once

#include <rapidxml/rapidxml.hpp>
#include <string>
#include <variant>

namespace GPE
{

class SavedScene
{
private:
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

public:
    enum class EType : uint16_t
    {
        BINARY,
        XML,
        NONE,
    };

    struct CreateArg
    {
        std::string data;
        EType       type;
    };

private:
    std::variant<XmlData> info;

    EType type = EType::NONE;

public:
    SavedScene(const CreateArg& arg);
    SavedScene()                  = default;
    SavedScene(const SavedScene&) = delete;
    SavedScene(SavedScene&&)      = default;

    SavedScene& operator=(SavedScene&&) = default;
    SavedScene& operator=(const SavedScene&) = delete;

    void        loadFromMemory(const char* data, size_t loadedSize, EType loadedType);
    std::string saveToMemory(EType& savedType);
};

} // namespace GPE