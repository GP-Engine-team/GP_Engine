/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

namespace GPE
{
// singletone
class ShaderParser
{
public:
    ShaderParser()                          = delete;
    ShaderParser(const ShaderParser& other) = delete;
    ShaderParser(ShaderParser&& other)      = delete;
    ~ShaderParser()                         = delete;

    /**
     * @brief call and use all parse function
     *
     * @param sourceCode
     */
    static inline void parse(std::string& sourceCode)
    {
        includeHeaders(sourceCode);
    }

    /**
     * @brief Search and include shader source code if the shader contain #include "path"
     *
     * @param sourceCode
     */
    static void includeHeaders(std::string& sourceCode);
};
} /*namespace GPE*/