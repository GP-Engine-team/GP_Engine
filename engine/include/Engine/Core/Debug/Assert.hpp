/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#ifndef NDEBUG
#include <string>

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Tools/Format.hpp"
#endif

namespace GPE
{
#ifdef NDEBUG

#define GPE_ASSERT(expr, msg)

#else /* Not NDEBUG.  */

#define GPE_ASSERT(expr, msg)                                                                                                  \
    if (expr)                                                                                                                  \
    {                                                                                                                          \
    }                                                                                                                          \
    else                                                                                                                       \
    {                                                                                                                          \
        GPE::Log::logError(GPE::stringFormat("%s in function %s %s : %d\nExpression \"%s\" == false.\n%s", F_RED("Assertion"), \
                           __FUNCSIG__, __FILE__, __LINE__, BOLD(#expr), msg));                                                \
        GPE::Log::closeAndTryToCreateFile();                                                                                   \
        exit(3);                                                                                                               \
    }

#endif // NDEBUG
} // namespace GPE