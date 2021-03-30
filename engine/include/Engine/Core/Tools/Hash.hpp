/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

namespace GPE
{

size_t constexpr mix(char m, size_t s)
{
    return ((s << 7) + ~(s >> 3)) + ~m;
}

/**
 * @brief Constexpr hash that allow user to use string in switch
 * @param m
 * @return
 */
size_t constexpr hash(const char* m)
{
    return (*m) ? mix(*m, hash(m + 1)) : 0;
}

} // namespace GPE