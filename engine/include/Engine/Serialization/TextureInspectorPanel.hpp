/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/IInspectable.hpp"

#include "Generated/TextureInspectorPanel.rfk.h"

namespace GPE RFKNamespace()
{
    // Allow user to modify shader importation setting
    class RFKClass() TextureInspectorPanel : public IInspectable
    {
    protected:
        Texture::ImportArg m_config;
        Texture            m_texture;
        std::string        m_path;
        bool               m_isDirty = false;

    public:
        TextureInspectorPanel() = delete;

        TextureInspectorPanel(const std::string& inPath);

        void inspect(InspectContext & context) override;
        TextureInspectorPanel_GENERATED
    };
} // namespace )