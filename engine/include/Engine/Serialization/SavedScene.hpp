#pragma once

#include <memory>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <variant>

namespace GPE
{

class SavedScene
{
public:
    struct XmlData
    {
        std::unique_ptr<rapidxml::xml_document<>> doc;
        std::unique_ptr<char[]>                   data;
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

public:
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

    static void toDoc(rapidxml::xml_document<>& doc, std::unique_ptr<char[]>& buffer, const char* data, size_t loadedSize);
};

} // namespace GPE