/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <functional>
#include <string>

template <typename T, typename... U>
size_t getAddress(std::function<T(U...)> f)
{
    typedef T(fnType)(U...);
    fnType** fnPointer = f.template target<fnType*>();
    return (size_t)*fnPointer;
}

/**
 * @brief
 * @example :   cb += [](){ puts("test c2");};
 *              cb += std::bind(&test::te, t);
 *              cb -= std::bind(&test::te, t);
 *              cb();
 * @tparam T
 */
template <typename T, typename... U>
class Callback
{
    std::vector<std::function<T(U...)>> m_functions;

public:
    void operator+=(std::function<T(U...)> functToAdd)
    {
        m_functions.emplace_back(functToAdd);
    }

    // Remove all
    void operator-=(std::function<T(U...)> functToRemove)
    {
        for (std::function<T(U...)>& funct : m_functions)
        {
            if (getAddress(funct) == getAddress(functToRemove))
            {
                std::swap(m_functions.back(), funct);
                m_functions.pop_back();
            }
        }
    }

    void operator()()
    {
        for (std::function<T(U...)>& funct : m_functions)
            funct();
    }

    void clear()
    {
        m_functions.clear();
    }

    template <typename TClass>
    static std::function<T(U...)> make(T (TClass::*function)(U...), TClass* owner)
    {
        return std::bind(function, owner);
    }
};

using Event = Callback<void>;