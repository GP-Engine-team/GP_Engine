/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/TransformComponent.hpp"    //TransformComponent
#include <list>                              //std::list
#include <memory>                            //std::unique_ptr
#include <string>                            //std::string
#include <vector>                            //std::vector

namespace Engine::Intermediate
{
struct GameObjectCreateArg
{
    const char* name = "";
};

class Component;

class GameObject
{
protected:
    std::string m_name;
    TransformComponent m_transform;

    std::list<Component*> m_pComponents;
    std::string m_tag{"GameObject"};
    bool m_isDead{false}; // Flag that inform it parent that this transform must be destroy on update loop

public:
    GameObject* parent = nullptr;
    std::list<std::unique_ptr<GameObject>> children = {};

public:

    inline
    GameObject(const GameObjectCreateArg& arg)
        : m_name{arg.name}, 
        m_transform{*this}, 
        m_pComponents{}
    {}

    inline
    GameObject() : m_name{""}, m_transform{*this}, m_pComponents{}
    {
    }

    inline
    GameObject (const GameObject& other) noexcept			= default;

    constexpr inline
    GameObject (GameObject&& other) noexcept				= default;

    inline
    ~GameObject () noexcept				                    = default;

    inline
    GameObject& operator=(GameObject const& other) noexcept		= default;

    inline
    GameObject& operator=(GameObject && other) noexcept			= default;

    /**
     * @brief update entity and these child if current entity is dirty
     *
     */
    void updateSelfAndChild() noexcept;

    /**
     * @brief Force the update of entity without check if entity is dirty
     *
     */
    void forceUpdate() noexcept;
    
     /**
     * @brief Get the Name object
     * @return const char*
     */
    inline std::string getName() const noexcept
    {
        return m_name;
    }

    /**
     * @brief Set the Name object
     * @param newName
     */
    inline void setName(const char* newName) noexcept
    {
        m_name = newName;
    }

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
    T* getComponent() noexcept;

    /**
     * @brief Get the first gameObject with path in arg
     *
     * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3
     * @return GraphEntity&
     */
    GameObject* getChild(const std::string& path) noexcept;

    /**
     * @brief Destroye the first gameObject with path in arg
     *
     * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3
     * @return GraphEntity&
     */
    void destroyChild(const std::string& path) noexcept;

    /**
     * @brief destroy childen of gameobject
     *
     * @param GameObject
     */
    std::list<std::unique_ptr<GameObject>>::iterator destroyChild(GameObject* pGameObject) noexcept;

    std::list<std::unique_ptr<GameObject>>::iterator destroyChild(
        const std::list<std::unique_ptr<GameObject>>::iterator& it) noexcept;

    /**
     * @brief destroy component of gameobject
     *
     * @param Component
     */
    std::list<Component*>::iterator destroyComponent(Component* pComponent) noexcept;

    void setActive(bool newState);

    std::list<Component*>::iterator destroyComponent(
        const std::list<Component*>::iterator& it) noexcept;

    /**
     * @brief Destroy the element at the next frame whe scene graph is update.
     *
     */
    virtual void destroy() noexcept;

    /**
     * @brief Destroy the element now, don't way the next frame.
     * WARNING : if element his destroy during update of system, it can create conflict or segfault.
     * Use this function only if you are sur that the object don't depending af the current frame
     *
     */
    virtual void destroyImmediate() noexcept;

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

    bool operator==(GameObject const& other);

    template <typename T>
    std::vector<T*> getComponents();

    std::list<Component*>& getComponents() noexcept;
    const std::list<Component*>& getComponents() const noexcept;

    std::string getRelativePath();

    void setTag(const std::string& newTag);
    std::string& getTag();

    bool compareTag(const std::string& toCompare);
};
} // namespace Engine::Intermediate