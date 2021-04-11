/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <functional>
#include <string>
#include "Engine/Core/Tools/FunctionPtr.hpp"

#include "Engine/Serialization/xml/xmlSaver.hpp"
#include "Engine/Serialization/xml/xmlLoader.hpp"

/**
 * @brief
 * @example :   cb += GPE::Function::make(t, "test")
 *              cb -= GPE::Function::make(t, "test")
 *              cb();
 * @tparam T
 */
template <typename T, typename... U>
class Callback
{
private:
    std::vector<GPE::Function> m_functions;

public:
    void operator+=(GPE::Function functToAdd)
    {
        m_functions.emplace_back(functToAdd);
    }

    // Remove all
    void operator-=(const GPE::Function& functToRemove)
    {
        for (GPE::Function& funct : m_functions)
        {
            if (funct == functToRemove)
            {
                std::swap(m_functions.back(), funct);
                m_functions.pop_back();
            }
        }
    }

    void operator()()
    {
        for (GPE::Function& funct : m_functions)
            funct();
    }

    void clear()
    {
        m_functions.clear();
    }

    void load(XmlLoader& context)
    {
        GPE::load(context, m_functions, XmlLoader::LoadInfo{"m_functions", "T", 0});
    }

    void save(XmlSaver& context) const
    {
        GPE::save(context, m_functions, XmlSaver::SaveInfo{"m_functions", "T", 0});

    }
};

using Event = Callback<void>;