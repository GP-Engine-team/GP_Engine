/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#ifndef NDEBUG
#include "Engine/Core/Debug/Log.hpp"
#include <string>
#endif

namespace Engine::Core::Debug 
{
#ifdef NDEBUG

    #define GPE_assert(expr)

#else /* Not NDEBUG.  */


    #define GPE_assert(expr, msg) \
        if (expr) {}	    \
        else                \
        {                   \
            Engine::Core::Debug::Log::logError((std::string(FRED("Assertion")) + " in function " + __FUNCSIG__ + " " +  __FILE__ + ":" + std::to_string(__LINE__) + "\nExpression \"" + BOLD(#expr) + "\" == false.\n" + msg).c_str()); \
            exit (3);           \
        }

#endif //NDEBUG
} // namespace Engine::Core::Debug