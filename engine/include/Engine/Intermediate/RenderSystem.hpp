/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>
#include <map>
#include <stdint.h>

#include "Engine/Resources/shader.hpp"

#include "Engine/Resources/Model.hpp"
#include "Engine/Resources/Camera.hpp"
#include "Engine/Resources/Model.hpp"

namespace Engine::Intermediate
{
    class RendererSystem
    {
        protected:

        static std::vector<Engine::Resources::Model*> m_pModels;
        static unsigned int m_currentShaderId;
        static unsigned int m_currentTextureId;
        static unsigned int m_currentMeshId;
        static Engine::Resources::Shader* m_currentPShaderUse;
        static bool         m_currentBackFaceCullingModeEnable;

        protected:

        static void tryToBindShader     (Engine::Resources::Shader* pShader);
        static void tryToBindTexture    (unsigned int textureId);
        static void tryToBindMesh       (unsigned int meshId);
        static void tryToSetAlphaEnabled(bool alphaEnabled);
        static void tryToSetBackFaceCulling(bool useBackFaceCulling);

        static void resetCurrentRenderPassKey();
        public:

        RendererSystem ()					                            = delete;
        RendererSystem (const RendererSystem& other)			= delete;
        RendererSystem (RendererSystem&& other)				    = delete;
        virtual ~RendererSystem ()				                    = delete;
        RendererSystem& operator=(RendererSystem const& other)  = delete;
        RendererSystem& operator=(RendererSystem && other)		= delete;

        static
        void draw () noexcept;

        /**
         * @brief Add Model (using key word this) on the physic system. This object will be updated by the physic system
         * 
         * @param pModel 
         */
        static
        void addModel (Engine::Resources::Model* pModel) noexcept;

        static
        void updateModelPointer (Engine::Resources::Model* newPointerModel, Engine::Resources::Model* exPointerModel) noexcept;

        static
        void removeModel (Engine::Resources::Model* pModel) noexcept;
    };
} /*namespace Engine::Intermediate*/