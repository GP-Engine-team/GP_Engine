/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>
#include <vector>

#include "Engine/Core/Parsers/MTLParser.hpp"
#include "Engine/Resources/Type.hpp"

namespace Engine::Core::Parsers
{
    /**
     * @brief Parse Obj file and fill buffer if arg not Null
     * 
     * @param objPath 
     * @param attrib 
     * @param shape 
     * @param materials 
     */
    extern void loadObjWithMTL (const char*                                         objPath,
                                Engine::Resources::Attrib*                         attrib,
                                std::vector<Engine::Resources::Shape>*             shape,
                                std::vector<Engine::Resources::MaterialAttrib>*    materials) noexcept;

} // namespace Engine::Core::Parsers