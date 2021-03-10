#include "Engine/Serialization/xml/xmlUtilities.hpp"

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_print.hpp"
#include <iostream>

using namespace rapidxml;

std::string findAttribValue(xml_node<>* parentNode, const std::string& key)
{
    for (xml_attribute<>* attrib = parentNode->first_attribute(); attrib; attrib = attrib->next_attribute())
    {
        std::string name(attrib->name(), attrib->name_size());
        if (name == key)
        {
            return std::string(attrib->value(), attrib->value_size());
        }
    }

    return "";
}
