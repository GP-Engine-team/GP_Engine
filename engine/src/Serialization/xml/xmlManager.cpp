#include "Engine/Serialization/xml/xmlManager.hpp"
#include "RapidXML/rapidxml_print.hpp"
#include <iostream>

void XmlManager::print()
{
    std::cout << doc << std::endl;
}