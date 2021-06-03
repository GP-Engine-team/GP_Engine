/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <cstdio>

namespace GPE
{
class BinarySaver
{
public:
    struct SaveInfo
    {

    };

    FILE* file = nullptr;

    template<typename T>
    void saveAsBinary(const T& saved)
    {
        fwrite(&saved, sizeof(saved), 1, file);
    }

    template <typename T>
    void saveArray(const T* saved, size_t nbElements)
    {
        fwrite(saved, sizeof(*saved), nbElements, file);
    }
};
    
template <typename T>
void save(BinarySaver& context, const T& saved, const BinarySaver::SaveInfo* info)
{
    context.saveAsBinary(saved);
}

template <>
inline void save(BinarySaver& context, const std::string& saved, const BinarySaver::SaveInfo* info)
{
    context.saveAsBinary(size_t(saved.size() + 1));
    context.saveArray(saved.data(), saved.size() + 1);
}

template <typename KEY, typename VALUE>
void save(BinarySaver& context, const std::map<KEY, VALUE>& saved, const BinarySaver::SaveInfo* info)
{
    context.saveAsBinary(saved.size());
    for (auto& [boneName, boneInfo] : saved)
    {
        GPE::save(context, boneName, nullptr);
        GPE::save(context, boneInfo, nullptr);
    }
}

template <typename T>
void save(BinarySaver& context, const std::vector<T>& saved, const BinarySaver::SaveInfo* info)
{
    context.saveAsBinary(saved.size());
    for (const T& elem : saved)
    {
        GPE::save(context, elem, nullptr);
    }
}

}