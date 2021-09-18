/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/IInspectable.hpp"

#include "Generated/MeshInspectorPanel.rfk.h"

namespace GPE RFKNamespace()
{
    // Allow user to modify shader importation setting
    class RFKClass() MeshInspectorPanel : public IInspectable
    {
    protected:
        Mesh::CreateIndiceBufferArg m_config;
        std::string                 m_path;
        bool                        m_isDirty           = false;
        bool                        m_canSaveInHardDisk = false;

    public:
        MeshInspectorPanel() = delete;

        MeshInspectorPanel(const std::string& inPath);

        void inspect(InspectContext & context) override;
        MeshInspectorPanel_GENERATED
    };
} // namespace )

File_GENERATED