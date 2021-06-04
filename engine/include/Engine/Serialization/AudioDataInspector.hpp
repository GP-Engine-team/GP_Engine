/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/InspectContext.hpp>
#include <unordered_map>

namespace GPE
{
struct SourceData;

template <>
void DataInspector::inspect(GPE::InspectContext& context, std::unordered_map<std::string, SourceData>& inspected,
                            const char* name);

} // namespace GPE
