/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Engine/Core/Maths/Mat.hpp"
#include "Engine/Ressources/Type.hpp"
#include "Engine/Ressources/Shader_type.hpp"
#include "Engine/Ressources/Material.hpp"
#include "Engine/Core/Maths/Shape3D/Volume.hpp"

namespace Engine::Resources
{
    enum class BoundingVolume
    {
        NONE,
        SPHERE
    };

    typedef struct SMeshConstructorArg
    {
        std::string                             objName;
        std::vector<Engine::Core::Maths::Vec3>  vBuffer;
        std::vector<Engine::Core::Maths::Vec2>  vtBuffer;
        std::vector<Engine::Core::Maths::Vec3>  vnBuffer;
        std::vector<std::vector<Engine::Ressources::Indice>> iBuffer;
        BoundingVolume                                       boundingVolume {BoundingVolume::SPHERE};

    } MeshConstructorArg;

    class Mesh
    {
        protected:
    
            GLuint                                                  m_indexVAO;
            bool                                                    m_isLoadInGPU;

            std::string                                             m_objName;
            std::vector<Engine::Core::Maths::Vec3>                  m_vBuffer;
            std::vector<Engine::Core::Maths::Vec2>                  m_vtBuffer;
            std::vector<Engine::Core::Maths::Vec3>                  m_vnBuffer;
            std::vector<std::vector<Engine::Ressources::Indice>>    m_iBuffer;
            std::vector<std::string>                                m_idMaterial;
            BoundingVolume                                          m_boundingVolumeType          {BoundingVolume::NONE};
            std::unique_ptr<Engine::Core::Maths::Shape3D::Volume>   m_boundingVolume;
        
        private:
    
        //this function generate the normal of object. Must be use if obj file don't contain normal.
        void generateNormalAndLoadIndice(const std::vector<Shape>& shapes) noexcept;

        public :

        enum class Axis
        {
            X, 
            Y,
            Z
        };

        public:
        
            Mesh (const char* objPath, bool loadInGPU = true, BoundingVolume boundingVolumeType = BoundingVolume::SPHERE);
            Mesh (MeshConstructorArg meshArg, bool loadInGPU = true);
            Mesh (const Attrib& attrib, const std::vector<Shape>& shape, bool loadInGPU = true, BoundingVolume boundingVolumeType = BoundingVolume::SPHERE);

            Mesh (const Mesh& other)  = delete;
            Mesh (Mesh&& other)  = default;
            ~Mesh ();

            /**
             * @brief Load Mesh from CPU to GPU. This operation can be slow but use more faster the Mesh.
             * 
             */
            void loadInGPU () noexcept;
            void unloadFromGPU () noexcept;

            /**
             * @brief return true if Mesh is load in GPU and ready to use
             * 
             * @return true 
             * @return false 
             */
            bool isLoadInGPU () const noexcept { return m_isLoadInGPU; }

            /**
             * @brief Drawn Mesh
             * 
             */
            void draw () const noexcept;

            /**
             * @brief Create a plae object of radius 1 and return it mesh. Plane is centered on the origin
             * 
             * @param textureRepetition     : if > 1 : repetition else split
             * @param indexTexture          : index of texture if split
             * @return MeshConstructorArg 
             */
            static 
            MeshConstructorArg createPlane	(float textureRepetition = 1.f, unsigned int indexTextureX = 0, unsigned int indexTextureY = 0, Axis towardAxis = Axis::Y);

            /**
             * @brief Create a Cube object of radius 1 and return it mesh. Cube is centered on the origin
             * 
             * @return MeshConstructorArg 
             */
            static 
            MeshConstructorArg createCube	(float textureRepetition = 1.f);

            /**
             * @brief Create a Sphere object of radius 1 and return it mesh. Sphere is centered on the origin
             * 
             * @param latitudeCount     : number of vertex in latitude
             * @param longitudeCount    : number of vertex in longitude
             * @return MeshConstructorArg 
             */
            static
            MeshConstructorArg createSphere(int latitudeCount, int longitudeCount);

            /**
             * @brief Create a Cylindre object
             * 
             * @param prescision 
             * @return MeshConstructorArg 
             */
            static 
            MeshConstructorArg createCylindre(unsigned int prescision); //TODO:: add uv and backFace Culling (bad normal)

            /**
             * @brief return the id of Mesh load in GPU.
             * 
             * @return const GLuint* : nullptr if Mesh is not load in GPU
             */
            const GLuint*  getVAOId () noexcept { return &m_indexVAO; }


            const std::vector<Engine::Core::Maths::Vec3>&                getVertices()  const noexcept { return m_vBuffer;}
            const std::vector<Engine::Core::Maths::Vec2>&                getUV()        const noexcept { return m_vtBuffer;}
            const std::vector<Engine::Core::Maths::Vec3>&                getNormals()   const noexcept { return m_vnBuffer;}
            const std::vector<std::vector<Engine::Ressources::Indice>>&  getIndices ()  const noexcept { return m_iBuffer; }

            /**
             * @brief Get the Id Materials object
             * 
             * @return const std::vector<std::string>& 
             */
            const std::vector<std::string>& getIdMaterials () const noexcept { return m_idMaterial; }
            
            void generateBoundingSphere() noexcept;

            BoundingVolume getBoundingVolumeType () const noexcept { return m_boundingVolumeType; }
            const Engine::Core::Maths::Shape3D::Volume* getBoundingVolume () const noexcept { return m_boundingVolume.get(); }
    };
} /*namespace Engine::Resources*/