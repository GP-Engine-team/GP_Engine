/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */
#pragma once

#include <cstdint>

namespace GPE
{

/**
 * @brief This class allow user to use more easly binary mask thanks to enum or data like :
 *   enum EParam : uint16_t
 *   {
 *       POSITION     = (1u << 1),
 *       COLOR        = (1u << 2),
 *       ...
 *    };
 *
 * @tparam T
 */
template <typename T>
class BinaryMask
{
protected:
    T m_mask;

public:
    bool isSet(T param) const noexcept
    {
        return m_mask & param;
    }

    void add(T param) noexcept
    {
        m_mask |= param;
    }

    void remove(T param) noexcept
    {
        m_mask &= ~param;
    }

    void reverse(T param) noexcept
    {
        m_mask ^= param;
    }

    T get() const noexcept
    {
        m_mask;
    }
};

using U8BMask  = BinaryMask<std::uint8_t>;
using U16BMask = BinaryMask<std::uint16_t>;
using U32BMask = BinaryMask<std::uint32_t>;
using U64BMask = BinaryMask<std::uint64_t>;

} // namespace GPE
