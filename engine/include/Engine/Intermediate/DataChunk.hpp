/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Tools/ClassUtility.hpp"
#include <array>  //std::array
#include <vector> //std::vector
#include "Engine/Core/Tools/UnrolledListAllocator.hpp"

// In inl
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Tools/BranchPrediction.hpp"

namespace GPE
{

/**
 * @brief The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over. Naif singleton according with link below :
 * @see https://refactoring.guru/fr/design-patterns/singleton/cpp/example
 * @tparam T : Component stored type
 */
template <typename TStoredData, size_t TSize = 65536> // 64KiB = 65,536Ko
class DataChunk
{
    /**
     * The Singleton's constructor/destructor should always be private to
     * prevent direct construction/desctruction calls with the `new`/`delete`
     * operator.
     */
private:
    static DataChunk* m_pInstance;
    UnrolledListAllocator<TStoredData> allocator = UnrolledListAllocator<TStoredData>::fromNbBytes(TSize);

protected:
    constexpr DataChunk() noexcept;

    ~DataChunk() noexcept = default;

public:
    constexpr DataChunk(const DataChunk& other) noexcept = delete;

    constexpr DataChunk(DataChunk&& other) noexcept = delete;

    constexpr DataChunk& operator=(DataChunk const& other) noexcept = delete;

    constexpr DataChunk& operator=(DataChunk&& other) noexcept = delete;

    /**
     * @brief Create new component with given arguments
     *
     * @tparam Args
     * @param args
     * @return T&
     */
    template <typename... Args>
    TStoredData* add(Args&&... args) noexcept;

    void destroy(TStoredData* const dataToDestroy) noexcept;

    //GETTER_BY_REF(Data, m_datas);

    /**
     * @brief This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     *
     * @param value
     * @return DataChunk*
     */
    static DataChunk* getInstance() noexcept;
    static void       setInstance(DataChunk& ptr) noexcept;
};

template <typename TStoredData, size_t TSize>
DataChunk<TStoredData, TSize>* DataChunk<TStoredData, TSize>::m_pInstance{nullptr};

#include "DataChunk.inl"

} /*namespace GPE*/
