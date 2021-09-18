/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/IInspectable.hpp"

#include "Generated/TextureImporterSetting.rfk.h"

namespace GPE RFKNamespace()
{
    // Allow user to modify shader importation setting
    class RFKClass() TextureImporterSetting : public IInspectable
    {
    protected:
        TextureImportConfig m_config     = {};
        std::string         m_srcPath    = "";
        std::string         m_dstPath    = "";
        bool                m_isImported = false;

    public:
        TextureImporterSetting() = delete;

        TextureImporterSetting(const char* srcPath, const char* dstPath);

        void inspect(InspectContext & context) override;

        TextureImporterSetting_GENERATED
    };
} // namespace )

File_GENERATED