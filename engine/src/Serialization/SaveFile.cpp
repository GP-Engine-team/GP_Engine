#include "Engine/Serialization/SaveFile.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <sstream>

namespace GPE
{

void SaveFile::loadFromMemory(const unsigned char* data, size_t loadedSize, EType loadedType)
{

}

std::stringstream SaveFile::saveToMemory(size_t& savedSize, EType& savedType)
{
    savedType = type;

    std::string buffer;

    switch (type)
    {
    case EType::XML:
        std::stringstream ss;
        ss << data.xmlData.doc;
        break;
    }

    return buffer;
}

} // namespace GPE