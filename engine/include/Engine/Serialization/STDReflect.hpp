#pragma once

#include <string>
/* #include <Refureku/Object.h> */
#include "Refureku/TypeInfo/Archetypes/Class.h"
#include <list>
#include <vector>

#include "Refureku/TypeInfo/Archetypes/FundamentalArchetype.h"

namespace rfk
{
template <typename T>
inline Archetype const* getArchetype() noexcept;

template <>
inline rfk::Archetype const* rfk::getArchetype<std::string>() noexcept
{
    static rfk::FundamentalArchetype archetype("string", std::hash<std::string>()("string"), sizeof(std::string));

    return &archetype;
}
} // namespace rfk