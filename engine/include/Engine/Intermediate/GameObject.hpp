/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <list>   //std::list
#include <memory> //std::unique_ptr
#include <string> //std::string
#include <vector> //std::vector

#include "Engine/ECS/Component/TransformComponent.hpp" //TransformComponent

#include "Engine/Serialization/DataInspector.hpp"
#include "Engine/Serialization/IInspectable.hpp"
#include "Engine/Serialization/InspectContext.hpp"
#include "Engine/Serialization/STDReflect.hpp"

#include <Engine/Core/Tools/ClassUtility.hpp>

// in Inl
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Core/Tools/Format.hpp>
#include <Engine/Intermediate/DataChunk.hpp>

#include <Generated/GameObject.rfk.h>

namespace GPE RFKNamespace()
{
    template <>
    void DataInspector::inspect(GPE::InspectContext & context, class GameObject & inspected);

    class Scene;

    class RFKClass(Serialize(false)) GameObject : public IInspectable
    {
    public:
        using Children = std::list<GameObject*>;

        struct CreateArg
        {
            std::string                   name = "World";
            TransformComponent::CreateArg transformArg;
            GameObject*                   parent = nullptr;
        };

    protected:
        RFKField(Inspect(), Serialize()) std::string m_name;
        RFKField(Serialize()) TransformComponent*    m_pTransform;
        RFKField(Serialize()) std::list<Component*>  m_pComponents;
        RFKField(Inspect(), Serialize()) std::string m_tag{"GameObject"};
        RFKField(Serialize()) GameObject*            m_parent = nullptr;
        RFKField() unsigned int                      m_id;
        RFKField(Serialize()) bool                   m_isDead{
            false}; // Flag that inform it parent that this transform must be destroy on update loop
        RFKField(Serialize()) bool m_isActive = true;

        // ID counter
        static unsigned int m_currentID;

    public:
        RFKField(Serialize()) Scene*                 pOwnerScene = nullptr;
        RFKField(Serialize()) std::list<GameObject*> children    = {};

    public:
        GameObject(Scene & scene, const CreateArg& arg = GameObject::CreateArg{});
        ~GameObject() noexcept;

        GameObject();
        GameObject(const GameObject& other) = delete;            // TODO: when transform is available
        GameObject& operator=(GameObject const& other) = delete; // TODO

        GameObject(GameObject && other) = default;
        GameObject& operator=(GameObject&& other) = default;

        void moveTowardScene(Scene & newOwner) noexcept;

        /**
         * @brief update entity and their children if current entity is dirty
         *
         */
        void updateSelfAndChildren() noexcept;
        void updateSelfAndChildren(const GPM::Mat4 parentModelMatrix) noexcept;

        /**
         * @brief Force the update of entity without check if entity is dirty
         *
         */
        void forceUpdate() noexcept;
        void forceUpdate(const GPM::Mat4 parentModelMatrix) noexcept;

        /**
         * @brief Get the Name object
         * @return const char*
         */
        [[nodiscard]] inline const std::string& getName() const noexcept;

        [[nodiscard]] inline const GameObject* getParent() const noexcept;
        [[nodiscard]] inline GameObject*       getParent() noexcept;

        /**
         * @brief Set the parent and remove to parent the child. Warning if pNewParent == nullptr, the gameObject must
         * be remove manually or must be instancied somewhere. Normally parent is repsonsable to destroy it's child
         * @param newName
         */
        void setParent(GameObject * pNewParent) noexcept;

        /**
         * @brief Warning ! This function only set the parent ptr and do not remove the children ptr of this parent
         * @param newParent
         * @return
         */
        inline void forceSetParent(GameObject & newParent) noexcept;

        /**
         * @brief Set the Name object
         * @param newName
         */
        inline void setName(const char* newName) noexcept;

        inline bool isDead();

        /**
         * @brief Get the Transform object
         * @return const char*
         */
        [[nodiscard]] constexpr inline const TransformComponent& getTransform() const noexcept;

        /**
         * @brief Get the Transform object
         * @return const char*
         */
        [[nodiscard]] constexpr inline TransformComponent& getTransform() noexcept;

        /**
         * @brief add a Component to the gameobject
         *
         * @tparam T
         * @tparam Args
         * @param args
         */
        template <typename T, typename... Args>
        T& addComponent(Args && ... args) noexcept;

        template <typename T>
        T& getOrCreateComponent();

        inline Component* addExistingComponent(Component * pExistingComponent) noexcept;

        /**
         * @brief Get the first Component type object
         *
         * @tparam T
         * @return Component&
         */
        template <typename T>
        [[nodiscard]] inline T* getComponent() noexcept;

        /**
         * @brief Get the first gameObject with path in arg
         *
         * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3
         * @return GraphEntity&
         */
        [[nodiscard]] GameObject* getChild(const std::string& path) noexcept;

        /**
         * @brief Destroy the first gameObject with path in arg
         *
         * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3
         * @return GraphEntity&
         */
        void destroyChild(const std::string& path) noexcept; // TODO: Destroy immediate may be dangerous

        /**
         * @brief destroy childen of gameobject
         *
         * @param GameObject
         */
        std::list<GameObject*>::iterator destroyChild(GameObject *
                                                      pGameObject) noexcept; // TODO: Destroy immediate may be dangerous

        inline std::list<GameObject*>::iterator destroyChild(
            const std::list<GameObject*>::iterator& it) noexcept; // TODO: Destroy immediate may be dangerous

        template <typename TUniqueComponentType>
        void destroyUniqueComponentNow() noexcept;

        /**
         * @brief onent of gameobject
         *
         * @param Component
         */
        std::list<Component*>::iterator destroyComponent(
            Component * pComponent) noexcept; // TODO: Destroy immediate may be dangerous

        inline void setActive(bool newState);

        inline std::list<Component*>::iterator destroyComponent(
            const std::list<Component*>::iterator& it) noexcept; // TODO: Destroy immediate may be dangerous

        /**
         * @brief Destroy the element at the next frame whe scene graph is update.
         *
         */
        inline void destroy() noexcept;

        /**
         * @brief Destroy the element now, don't way the next frame.
         * WARNING : if element his destroy during update of system, it can create conflict or segfault.
         * Use this function only if you are sur that the object don't depending af the current frame
         *
         */
        inline void destroyNow() noexcept;

        /**
         * @brief add specific entity to the graph with arg to construct it and return his id
         *
         * @param args
         * @param dependenceEntity&
         * @return GameObject&
         */
        template <typename... Args>
        GameObject& addChild(Args && ... args) noexcept;

        // TODO: Remove this function when dataChunk will be rework
        /**
         * @brief Recursive function that allow user to find gameObject corresponding to Id
         * @param ID
         * @return
         */
        GameObject* getGameObjectCorrespondingToID(unsigned int ID) noexcept;

        /**
         * @brief Get the Entity object in function of path in arg
         *
         * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3
         * @return GameObject&
         */
        GameObject* getGameObject(const std::string& path) noexcept;

        [[nodiscard]] inline constexpr bool operator==(GameObject const& other) noexcept;

        template <typename T>
        [[nodiscard]] std::vector<T*> getComponents() noexcept;

        [[nodiscard]] inline constexpr std::list<Component*>& getComponents() noexcept;

        [[nodiscard]] inline constexpr const std::list<Component*>& getComponents() const noexcept;

        [[nodiscard]] std::string getAbsolutePath() const noexcept;

        [[nodiscard]] inline unsigned int getID() const noexcept;

        inline void setTag(const std::string& newTag) noexcept;

        [[nodiscard]] inline constexpr const std::string& getTag() const noexcept;

        [[nodiscard]] inline bool compareTag(const std::string& toCompare) const noexcept;

        void inspect(GPE::InspectContext & context) override;

        static void* operator new(std::size_t size);

        static void operator delete(void* ptr);

        GameObject_GENERATED
    };

    /**
     * @brief GameObjectLinker allow user to link local gameObject in editor. Thank's to it, user can associate
     * gameObject and serialize it. Warning ! : Assocation is local of the base scene. For prefab, the base scene is the
     * first gameObject. So user cannot link gameObject that is not inside the prefab.
     *
     */
    struct GameObjectLinker
    {
        class GameObject* pGo = nullptr;
    };

} // namespace )

#include "GameObject.inl"