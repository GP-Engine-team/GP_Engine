#pragma once

#include <string>
#include "rapidxml.hpp"

/**
 * @brief Finds the attribute value corresponding to a child with key name.
 * @param parentNode The direct children of this node will be searched.
 * @param key The name of the attribute you want to get the value from..
 * @return Returns the value of the searched attribute.
*/
std::string findAttribValue(rapidxml::xml_node<>* parentNode, const std::string& key);
