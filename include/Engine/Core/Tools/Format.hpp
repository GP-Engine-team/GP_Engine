/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>
#include <string>
#include <cassert>

namespace Engine::Core::Tools
{
    /**
     * @brief std::string formatting like sprintf. Use std::format if you use __cplusplus > 201703L
     * 
     * @link https://stackoverflow.com/a/26221725/12703286
     * @tparam Args 
     * @param format 
     * @param args 
     * @return std::string 
     */
    template<typename ... Args>
    std::string stringformat( const std::string& format, Args ... args )
    {
        int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
        
        assert(size > 0, "Error during formatting.");

        std::unique_ptr<char[]> buf( new char[ size ] ); 
        snprintf( buf.get(), size, format.c_str(), args ... );
        
        return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    }
    
} /*namespace Engine::Core::Tools*/

