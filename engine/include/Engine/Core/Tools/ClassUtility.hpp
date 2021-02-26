/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once

#define GETTER_BY_VALUE(variableName, variable)                                                                        \
    [[nodiscard]] inline constexpr decltype(variable) get##variableName() const noexcept                               \
    {                                                                                                                  \
        return variable;                                                                                               \
    }

#define GETTER_BY_REF(variableName, variable)                                                                          \
    [[nodiscard]] inline constexpr decltype(variable)& get##variableName() noexcept                                    \
    {                                                                                                                  \
        return variable;                                                                                               \
    }

#define GETTER_BY_CONST_REF(variableName, variable)                                                                    \
    [[nodiscard]] inline constexpr const decltype(variable)& get##variableName() const noexcept                        \
    {                                                                                                                  \
        return variable;                                                                                               \
    }

#define SETTER_BY_CONST_REF(variableName, variable)                                                                    \
    inline constexpr void set##variableName(const decltype(variable)& in_##variableName) noexcept                      \
    {                                                                                                                  \
        variable = in_##variableName;                                                                                  \
    }

#define SETTER_BY_VALUE(variableName, variable)                                                                        \
    inline constexpr void set##variableName(const decltype(variable)& in_##variableName) noexcept                      \
    {                                                                                                                  \
        variable = in_##variableName;                                                                                  \
    }

#define DEFAULT_GETTER_SETTER(variableName, variable)                                                                  \
    GETTER_BY_CONST_REF(variableName, variable)                                                                        \
    SETTER_BY_CONST_REF(variableName, variable)
