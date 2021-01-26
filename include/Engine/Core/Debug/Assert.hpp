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

#include "Engine/Core/Tools/Format.hpp"

namespace Engine::Core::Debug 
{
#ifdef NDEBUG

    #define GPE_ASSERT(expr)

#else /* Not NDEBUG.  */


    #define GPE_ASSERT(expr, msg) \
        if (expr) {}	    \
        else                \
        {                   \
            Engine::Core::Debug::Log::logError(Engine::Core::Tools::stringformat("%s in function %s %s : %d\nExpression \"%s\" == false.\n%s", FRED("Assertion"), __FUNCSIG__, __FILE__, __LINE__, BOLD(#expr), msg)); \
            Engine::Core::Debug::Log::closeAndTryToCreateFile();    \
            exit (3);       \
        }

#endif //NDEBUG
} // namespace Engine::Core::Debug