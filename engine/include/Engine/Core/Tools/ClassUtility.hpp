/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once

#define DEFAULT_GETTER(variableName, variable)                                                                         \
    [[nodiscard]] inline constexpr decltype(variable) &get##variableName() noexcept                                    \
    {                                                                                                                  \
        return variable;                                                                                               \
    }

#define BY_VALUE_GETTER(variableName, variable)                                                                        \
    [[nodiscard]] inline constexpr decltype(variable) get##variableName() const noexcept                               \
    {                                                                                                                  \
        return variable;                                                                                               \
    }

#define DEFAULT_CONST_GETTER(variableName, variable)                                                                   \
    [[nodiscard]] inline constexpr const decltype(variable) &get##variableName() const noexcept                        \
    {                                                                                                                  \
        return variable;                                                                                               \
    }

#define DEFAULT_SETTER(variableName, variable)                                                                         \
    inline constexpr void set##variableName(const decltype(variable) &in_##variableName) noexcept                      \
    {                                                                                                                  \
        variable = in_##variableName;                                                                                  \
    }

#define DEFAULT_GETTER_SETTER(variableName, variable)                                                                  \
    DEFAULT_CONST_GETTER(variableName, variable)                                                                       \
    DEFAULT_SETTER(variableName, variable)
