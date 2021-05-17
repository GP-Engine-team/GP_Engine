/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "RapidXML/rapidxml.hpp"
#include <string>

/**
 * @brief Finds the attribute value corresponding to a child with key name.
 * @param parentNode The direct children of this node will be searched.
 * @param key The name of the attribute you want to get the value from..
 * @return Returns the value of the searched attribute.
 */
std::string findAttribValue(rapidxml::xml_node<>* parentNode, const std::string& key);

std::string docToString(const rapidxml::xml_document<>& doc);