/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/SpacialAttribut.hpp"

namespace Engine::Intermediate
{
    class LocalTransform
    {
        protected:
    
            SpacialAttribut m_spacialAttribut;

        public:
            
            constexpr inline
            LocalTransform () noexcept 					            = default;

            constexpr inline
            LocalTransform (const LocalTransform& other) noexcept	= default;

            constexpr inline
            LocalTransform (LocalTransform&& other) noexcept		= default;

            inline
            ~LocalTransform () noexcept				                = default;

            constexpr inline
            LocalTransform& operator=(LocalTransform const& other) noexcept		= default;

            constexpr inline
            LocalTransform& operator=(LocalTransform && other) noexcept			= default;

            LocalTransform ( const GPM::Vec3& position, 
                        const GPM::Vec3& rotation, 
                        const GPM::Vec3& scale)
                : m_position (position),
                  m_rotation (rotation),
                  m_scale    (scale)
            {}

            LocalTransform (const LocalTransformCreateArg& arg)
                : m_position (arg.position),
                  m_rotation (arg.rotation),
                  m_scale    (arg.scale)
            {}

            [[nodiscard]] constexpr inline
            const GPM::Vec3& getPosition() const noexcept   { return m_position; }

                        [[nodiscard]] constexpr inline
            const GPM::Vec3& getRotation() const noexcept   { return m_rotation; }

                        [[nodiscard]] constexpr inline
            const GPM::Vec3& getScale() const noexcept      { return m_scale; }

            constexpr inline
            void rotate (float angleRad, GPM::Vec3 rotAxis) noexcept
            {
                rotAxis.normalize();
                m_rotation += {rotAxis.x * angleRad, rotAxis.y * angleRad, rotAxis.z * angleRad};
            }

            constexpr inline
            void rotate (const GPM::Vec3& newRotation) noexcept
            {
                m_rotation += newRotation;
            }

            constexpr inline
            void translate (const GPM::Vec3& translation) noexcept
            {
                m_position += translation;
            }

            constexpr inline
            void scale (const GPM::Vec3& scale) noexcept
            {
                m_scale += scale;
            }

            constexpr inline
            void setRotation (float angleRad, GPM::Vec3 rotAxis) noexcept
            {
                rotAxis.normalize();
                m_rotation = {rotAxis.x * angleRad, rotAxis.y * angleRad, rotAxis.z * angleRad};
            }

            constexpr inline
            void setRotation (const GPM::Vec3& rotation) noexcept
            {
                m_rotation = rotation;
            }

            constexpr inline
            void setTranslation (const GPM::Vec3& translation) noexcept
            {
                m_position = translation;
            }

            constexpr inline
            void setScale (const GPM::Vec3& scale) noexcept
            {
                m_scale = scale;
            }

            #pragma endregion //!mutator   

    };
} /*namespace Engine::Intermediate*/
