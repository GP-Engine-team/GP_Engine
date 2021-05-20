/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/IInspectable.hpp"

#include "Generated/MaterialInspectorPanel.rfk.h"

namespace GPE RFKNamespace()
{

    // Allow user to modify shader importation setting
    class RFKClass() MaterialInspectorPanel : public IInspectable
    {
    protected:
        Material::ImporteArg m_config;
        std::string          m_path;
        bool                 m_isDirty           = false;
        bool                 m_canSaveInHardDisk = false;

    public:
        MaterialInspectorPanel() = delete;
        MaterialInspectorPanel(const std::string& inPath);

        void inspect(InspectContext & context) override;

        MaterialInspectorPanel_GENERATED
    };

} // namespace )