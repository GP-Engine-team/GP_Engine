/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/TransformComponent.hpp" //TransformComponent
#include <list>                                        //std::list
#include <memory>                                      //std::unique_ptr
#include <string>                                      //std::string
#include <vector>                                      //std::vector

// in Inl
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Tools/Format.hpp"
#include "Engine/Intermediate/DataChunk.hpp"

namespace GPE
{
class Scene;

class GameObject
{
public:
    struct CreateArg
    {
        std::string                   name = "World";
        TransformComponent::CreateArg transformArg;
        GameObject*                   parent = nullptr;
    };

protected:
    std::string         m_name;
    TransformComponent& m_transform;

    std::list<Component*> m_pComponents;
    std::string           m_tag{"GameObject"};
    bool        m_isDead{false}; // Flag that inform it parent that this transform must be destroy on update loop
    GameObject* m_parent = nullptr;

public:
    Scene*                                 pOwnerScene;
    std::list<std::unique_ptr<GameObject>> children = {};

public: // TODO : Protected method ?
    template <typename T>
    void updateComponentLink(const T* oldPtr, T* newPtr) noexcept;

public:
    inline GameObject(Scene& scene, const CreateArg& arg = GameObject::CreateArg{});
    virtual ~GameObject() noexcept;

    GameObject()                        = delete;
    GameObject(const GameObject& other) = delete;            // TODO: when transform is available
    GameObject& operator=(GameObject const& other) = delete; // TODO

    inline GameObject(GameObject&& other) = default;
    inline GameObject& operator=(GameObject&& other) noexcept = default;

    void moveTowardScene(Scene& newOwner) noexcept;

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
     * @brief Set the parent and remove to parent the child
     * @param newName
     */
    inline void setParent(GameObject& newParent) noexcept;

    /**
     * @brief Set the Name object
     * @param newName
     */
    inline void setName(const char* newName) noexcept;

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
    T& addComponent(Args&&... args) noexcept;

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
     * @brief Destroye the first gameObject with path in arg
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
    std::list<std::unique_ptr<GameObject>>::iterator destroyChild(
        GameObject* pGameObject) noexcept; // TODO: Destroy immediate may be dangerous

    inline std::list<std::unique_ptr<GameObject>>::iterator destroyChild(
        const std::list<std::unique_ptr<GameObject>>::iterator&
            it) noexcept; // TODO: Destroy immediate may be dangerous

    template <typename TUniqueComponentType>
    void destroyUniqueComponentNow() noexcept;

    /**
     * @brief destroy component of gameobject
     *
     * @param Component
     */
    std::list<Component*>::iterator destroyComponent(
        Component* pComponent) noexcept; // TODO: Destroy immediate may be dangerous

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
     * @tparam T
     * @tparam Args
     * @param args
     * @param dependenceEntity&
     * @return GameObject&
     */
    template <typename T, typename... Args>
    GameObject& addChild(Args&&... args) noexcept;

    [[nodiscard]] inline constexpr bool operator==(GameObject const& other) noexcept;

    template <typename T>
    [[nodiscard]] std::vector<T*> getComponents() noexcept;

    [[nodiscard]] inline constexpr std::list<Component*>& getComponents() noexcept;

    [[nodiscard]] inline constexpr const std::list<Component*>& getComponents() const noexcept;

    [[nodiscard]] std::string getAbsolutePath() const noexcept;

    inline void setTag(const std::string& newTag) noexcept;

    [[nodiscard]] inline constexpr const std::string& getTag() const noexcept;

    [[nodiscard]] inline bool compareTag(const std::string& toCompare) const noexcept;
};

#include "GameObject.inl"

} // namespace GPE
