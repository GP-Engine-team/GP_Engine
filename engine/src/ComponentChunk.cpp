
#include "Engine/Intermediate/ComponentChunk.hpp"

using namespace Engine::Intermediate;

template<typename TStoredComponent>
ComponentChunk<TStoredComponent>* ComponentChunk<TStoredComponent>::m_pInstance{nullptr};

template<typename TStoredComponent>
std::shared_mutex ComponentChunk<TStoredComponent>::m_mutex;