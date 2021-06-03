/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once


#include <string>
#include <map>
#include <memory>
#include <cstdio>

namespace GPE
{
class BinaryLoader
{
public:
    struct LoadInfo
    {
    };

    FILE* file = nullptr;

    template <typename T>
    void loadAsBinary(T& loaded)
    {
        fread(&loaded, sizeof(loaded), 1, file);
    }

    template <typename T>
    void loadArray(T* loaded, size_t nbElements)
    {
        fread(loaded, sizeof(*loaded), nbElements, file);
    }
};

template <typename T>
void load(BinaryLoader& context, T& loaded, const BinaryLoader::LoadInfo* info)
{
    context.loadAsBinary(loaded);
}

template <>
inline void load(BinaryLoader& context, std::string& loaded, const BinaryLoader::LoadInfo* info)
{
    size_t size;
    context.loadAsBinary(size);
    std::unique_ptr<char[]> str = std::make_unique<char[]>(size);
    context.loadArray(str.get(), size);
    loaded = str.get();
}

template <typename KEY, typename VALUE>
void load(BinaryLoader& context, std::map<KEY, VALUE>& loaded, const BinaryLoader::LoadInfo* info)
{
    size_t size;
    context.loadAsBinary(size);
    for (size_t i = 0; i < size; i++)
    {
        KEY key;
        VALUE value;
        GPE::load(context, key, nullptr);
        GPE::load(context, value, nullptr);
        loaded.emplace(key, value);
    }
}

template <typename T>
void load(BinaryLoader& context, std::vector<T>& loaded, const BinaryLoader::LoadInfo* info)
{
    size_t size;
    context.loadAsBinary(size);
    loaded.resize(size);

    for (size_t i = 0; i < size; i++)
    {
        GPE::load(context, loaded[i], nullptr);
    }
}

} // namespace GPE