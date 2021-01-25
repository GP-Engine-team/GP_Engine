#pragma once

#ifndef NDEBUG
#include "Engine/Core/Debug/Log.hpp"
#include <string>
#endif

namespace Engine::Core::Debug 
{
#ifdef NDEBUG

    #define GPE_assert(expr)
    #define GPE_assertInfo(expr, msg)

#else /* Not NDEBUG.  */

    #define GPE_assert(expr) \
        if (expr) {}	    \
        else                \
        {                   \
            Engine::Core::Debug::Log::logError((std::string(FRED("Assertion")) + " in function " + __FUNCTION__ + " " +  __FILE__ + ":" + std::to_string(__LINE__) + "\nExpression \"" + BOLD(#expr) + "\" == false.\n").c_str()); \
            exit (3);       \
        }

    #define GPE_assertInfo(expr, msg) \
        if (expr) {}	    \
        else                \
        {                   \
            Engine::Core::Debug::Log::logError((std::string(FRED("Assertion")) + " in function " + __FUNCTION__ + " " +  __FILE__ + ":" + std::to_string(__LINE__) + "\nExpression \"" + BOLD(#expr) + "\" == false.\n" + msg).c_str()); \
            exit (3);           \
        }

#endif //NDEBUG
} // namespace Engine::Core::Debug