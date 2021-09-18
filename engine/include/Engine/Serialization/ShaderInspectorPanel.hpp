/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/IInspectable.hpp"
#include <string>

#include "Generated/ShaderInspectorPanel.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass() ShaderInspectorPanel : public IInspectable // Allow user to modify shader importation setting
    {
    protected:
        std::string        m_path = nullptr;
        ShaderCreateConfig m_config;
        bool               m_isDirty = false;

    public:
        ShaderInspectorPanel() = delete;

        ShaderInspectorPanel(const std::string& inPath);

        void inspect(InspectContext & context) override;

        ShaderInspectorPanel_GENERATED
    };
} // namespace )

File_GENERATED