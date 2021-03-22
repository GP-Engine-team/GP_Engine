#include "Engine/Intermediate/DataChunk.hpp"

template <typename TStoredData, int TSize>
constexpr inline DataChunk<TStoredData, TSize>::DataChunk() noexcept
{
    m_datas.reserve(TSize / sizeof(TStoredData));
}

template <typename TStoredData, int TSize>
template <typename... Args>
TStoredData& DataChunk<TStoredData, TSize>::add(Args&&... args) noexcept
{
    return m_datas.emplace_back(std::forward<Args>(args)...);
}

template <typename TStoredData, int TSize>
void DataChunk<TStoredData, TSize>::destroy(const TStoredData* dataToDestroy) noexcept
{
    for (size_t i = 0; i < m_datas.size(); ++i)
    {
        if (unlikely(&m_datas[i] == dataToDestroy))
        {
            TStoredData tmp = std::move(m_datas[i]);
            m_datas[i]     = std::move(m_datas.back());
            m_datas.back() = std::move(tmp);

            m_datas.pop_back();
        }
    }
}

template <typename TStoredData, int TSize>
DataChunk<TStoredData, TSize>* DataChunk<TStoredData, TSize>::getInstance() noexcept
{
    if (unlikely(m_pInstance == nullptr))
    {
        m_pInstance = new DataChunk<TStoredData, TSize>();
    }

    return m_pInstance;
}

template <typename TStoredData, int TSize>
void DataChunk<TStoredData, TSize>::setInstance(DataChunk& ptr) noexcept
{
    m_pInstance = &ptr;
}