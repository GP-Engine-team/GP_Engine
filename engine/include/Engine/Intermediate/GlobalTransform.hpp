/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/LocalTransform.hpp"

namespace Engine::Intermediate
{
    class GlobalTransform : public LocalTransform
    {
        protected:

            GPM::Mat4 m_modelMat; //Local coordinates are the coordinates of object relative to its local origin; they're the coordinates object begins in.
            bool                      m_isDirty; //Flag on if transform class must be update    
    
        public:

            constexpr inline
            GlobalTransform () noexcept 					            = default;

            constexpr inline
            GlobalTransform (const GlobalTransform& other) noexcept	= default;

            constexpr inline
            GlobalTransform (GlobalTransform&& other) noexcept		= default;

            inline
            ~GlobalTransform () noexcept				                = default;

            constexpr inline
            GlobalTransform& operator=(GlobalTransform const& other) noexcept		= default;

            constexpr inline
            GlobalTransform& operator=(GlobalTransform && other) noexcept			= default;


            GlobalTransform ( const GPM::Vec3& position, 
                        const GPM::Vec3& rotation, 
                        const GPM::Vec3& scale)
                : LocalTransform (position, rotation, scale),
                  m_modelMat (),
                  m_isDirty  (true)
            {
                update();
            }

            GlobalTransform (const GlobalTransformCreateArg& arg)
                : LocalTransform (arg.position, arg.rotation, arg.scale)
            {}

            /**
             * @brief update Mesh matrix 
             * 
                        
                        constexpr inline */
            void update () noexcept
            {
                if (!m_isDirty)
                    return;

                m_modelMat = GPM::Mat4::createTRSMatrix(m_scale, m_rotation, m_position);
                m_isDirty = false;
            }

            /**
             * @brief update transform if it depend to parent Mesh view (use in inherance in scene graph)
             * 
             * @param parentMeshMatrix : Mesh view matrix of parent
                        
                        constexpr inline */
            void update (const GPM::Mat4& parentMeshMatrix) noexcept
            {
                m_modelMat = parentMeshMatrix * GPM::Mat4::createTRSMatrix(m_scale, m_rotation, m_position);
                m_isDirty = false;  
            }

            [[nodiscard]] constexpr inline
            const GPM::Mat4& getModelMatrix () const noexcept { return m_modelMat; }

            /**
             * @brief return true if object was modify and must by update
             * 
             * @return true 
             * @return false 
             */
                        [[nodiscard]] constexpr inline
            bool isDirty() const noexcept { return m_isDirty; }

            [[nodiscard]] constexpr inline
            const GPM::Vec3 getVecForward() const noexcept  { return m_modelMat.getVectorForward().getNormalize(); }
                        [[nodiscard]] constexpr inline
            const GPM::Vec3 getVecRight() const noexcept    { return m_modelMat.getVectorRight().getNormalize(); }
                        [[nodiscard]] constexpr inline
            const GPM::Vec3 getVecUp() const noexcept       { return m_modelMat.getVectorUp().getNormalize(); }

            constexpr inline
            void setVecForward(const GPM::Vec3& newForward)  noexcept 
            { 
                m_modelMat.setVectorForward(newForward.getNormalize());
                m_modelMat.setVectorRight(m_modelMat.getVectorForward().getCross(m_modelMat.getVectorUp()).normalize());
                m_modelMat.setVectorUp(m_modelMat.getVectorForward().getCross(m_modelMat.getVectorRight()).normalize());
                m_modelMat *= GPM::Mat4::createScaleMatrix (m_scale);
            }

            constexpr inline
            void setVecRight(const GPM::Vec3& newRight)  noexcept
            {
                m_modelMat.setVectorRight(newRight.getNormalize());
                m_modelMat.setVectorForward(m_modelMat.getVectorRight().getCross(m_modelMat.getVectorUp()).normalize());
                m_modelMat.setVectorUp(m_modelMat.getVectorForward().getCross(m_modelMat.getVectorRight()).normalize()); 
                m_modelMat *= GPM::Mat4::createScaleMatrix (m_scale);  
            }

            constexpr inline
            void setVecUp(const GPM::Vec3& newUp)  noexcept
            {
                m_modelMat.setVectorUp(newUp.getNormalize());
                m_modelMat.setVectorForward(m_modelMat.getVectorRight().getCross(m_modelMat.getVectorUp()).normalize());
                m_modelMat.setVectorRight(m_modelMat.getVectorForward().getCross(m_modelMat.getVectorUp()).normalize());
                m_modelMat *= GPM::Mat4::createScaleMatrix (m_scale);
            }

            constexpr inline
            void rotate (float angleRad, GPM::Vec3 rotAxis) noexcept
            {
                rotAxis.normalize();
                m_rotation += {rotAxis.x * angleRad, rotAxis.y * angleRad, rotAxis.z * angleRad};
                m_isDirty = true;
            }

            constexpr inline
            void rotate (const GPM::Vec3& newRotation) noexcept
            {
                m_rotation += newRotation;
                m_isDirty = true;
            }

            constexpr inline
            void translate (const GPM::Vec3& translation) noexcept
            {
                m_position += translation;
                m_isDirty = true;
            }

            constexpr inline
            void scale (const GPM::Vec3& scale) noexcept
            {
                m_scale += scale;
                m_isDirty = true;
            }

            constexpr inline
            void setRotation (float angleRad, GPM::Vec3 rotAxis) noexcept
            {
                rotAxis.normalize();
                m_rotation = {rotAxis.x * angleRad, rotAxis.y * angleRad, rotAxis.z * angleRad};
                m_isDirty = true;
            }

            constexpr inline
            void setRotation (const GPM::Vec3& rotation) noexcept
            {
                m_rotation = rotation;
                m_isDirty = true;
            }

            constexpr inline
            void setTranslation (const GPM::Vec3& translation) noexcept
            {
                m_position = translation;
                m_isDirty = true;
            }

            constexpr inline
            void setScale (const GPM::Vec3& scale) noexcept
            {
                m_scale = scale;
                m_isDirty = true;
            }

            [[nodiscard]] constexpr inline
            GPM::Vec3 getGlobalPosition() const noexcept   { return {m_modelMat[3][0], m_modelMat[3][1], m_modelMat[3][2]}; }
                       [[nodiscard]] constexpr inline
            GPM::Vec3 getGlobalScale() const noexcept      { return {m_modelMat.getVectorRight().length(), m_modelMat.getVectorUp().length(), m_modelMat.getVectorForward().length()}; }

            /**
             * @brief Set the Dirty object and force it to be update
             * 
                        
                        constexpr inline */
            void setDirty()  noexcept { m_isDirty = true;}

    };
} /*namespace Engine::Intermediate*/
