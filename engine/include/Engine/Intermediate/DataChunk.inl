#include "Engine/Intermediate/DataChunk.hpp"

template <typename TStoredData, size_t TSize>
constexpr inline DataChunk<TStoredData, TSize>::DataChunk() noexcept
{
    //m_datas.reserve(TSize / sizeof(TStoredData));
}

template <typename TStoredData, size_t TSize>
template <typename... Args>
TStoredData* DataChunk<TStoredData, TSize>::add(Args&&... args) noexcept
{
    TStoredData* instance = allocator.allocate(1); // not constructed yet
    return new (instance) TStoredData(std::forward<Args>(args)...);
}

template <typename TStoredData, size_t TSize>
void DataChunk<TStoredData, TSize>::destroy(TStoredData* const dataToDestroy) noexcept
{
    dataToDestroy->~TStoredData();
    allocator.deallocate(dataToDestroy);
    //delete dataToDestroy;
}

template <typename TStoredData, size_t TSize>
DataChunk<TStoredData, TSize>* DataChunk<TStoredData, TSize>::getInstance() noexcept
{
    if (unlikely(m_pInstance == nullptr))
    {
        m_pInstance = new DataChunk<TStoredData, TSize>();
    }

    return m_pInstance;
}

template <typename TStoredData, size_t TSize>
void DataChunk<TStoredData, TSize>::setInstance(DataChunk& ptr) noexcept
{
    m_pInstance = &ptr;
}