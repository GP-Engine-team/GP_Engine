/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/IInspectable.hpp"
#include "Engine/Resources/Importer/Importer.hpp"
#include "Generated/MaterialImporterSetting.rfk.h"

namespace GPE RFKNamespace()
{

// Allow user to modify shader importation setting
class RFKClass() MaterialImporterModifier : public IInspectable
{
protected:
    Material::ImporteArg m_config;
    std::string          m_path;
    bool                 m_isDirty;

public:
    MaterialImporterModifier() = delete;
    MaterialImporterModifier(const std::string& inPath);

    void inspect(InspectContext& context) override;

    MaterialImporterModifier_GENERATED
};

} // namespace GPE