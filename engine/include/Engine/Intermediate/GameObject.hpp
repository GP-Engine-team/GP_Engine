//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 19 h 27

#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <list> //std::list
#include <vector> //std::vector
#include <string> //std::string
#include <memory> //std::unique_ptr
#include "Engine/Intermediate/Entity.hpp" //Entity
#include "Engine/Intermediate/Component.hpp" //Component

namespace Engine::Intermediate
{
    typedef EntityCreateArg GameObjectCreateArg;

    class GameObject : public Entity
    {
    protected:

        std::list<std::unique_ptr<Component>>   m_components;
        std::string                             m_tag{"GameObject"};
        bool                                    m_isDead {false}; //Flag that inform it parent that this transform must be destroy on update loop

    public :
        GameObject* parent;
        std::list<std::unique_ptr<GameObject>> children;

    public:

        GameObject(const GameObjectCreateArg& arg)
            :  Entity {arg}
        {}

        GameObject()                                        = default;
        GameObject(const GameObject &other)                 = default;
        GameObject(GameObject &&other)                      = default;
        virtual ~GameObject()                               = default;
        GameObject &operator=(GameObject const &other)      = default;
        GameObject &operator=(GameObject &&other)           = default;

        /**
         * @brief update entity and these child if current entity is dirty
         * 
         */
        virtual void updateSelfAndChild() noexcept;

        /**
         * @brief Force the update of entity without check if entity is dirty
         * 
         */
        void forceUpdate();

        /**
         * @brief add a Component to the gameobject
         * 
         * @tparam T 
         * @tparam Args 
         * @param args 
         */
        template <typename T, typename... Args>
        T& addComponent(Args &&... args) noexcept;

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
        GameObject* getChild (const std::string& path) noexcept;

        /**
         * @brief Destroye the first gameObject with path in arg
         * 
         * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3 
         * @return GraphEntity& 
         */
        void destroyChild (const std::string& path) noexcept;

        /**
         * @brief destroy childen of gameobject
         * 
         * @param GameObject 
         */
        std::list<std::unique_ptr<GameObject>>::iterator destroyChild (GameObject* pGameObject) noexcept;

        std::list<std::unique_ptr<GameObject>>::iterator destroyChild (const std::list<std::unique_ptr<GameObject>>::iterator& it) noexcept;

        /**
         * @brief destroy component of gameobject
         * 
         * @param Component 
         */
        std::list<std::unique_ptr<Component>>::iterator destroyComponent (Component* pComponent) noexcept;

        void setActive(bool newState);

        std::list<std::unique_ptr<Component>>::iterator destroyComponent (const std::list<std::unique_ptr<Component>>::iterator& it) noexcept;

        /**
         * @brief Destroy the element at the next frame whe scene graph is update. 
         * 
         */
        virtual 
        void destroy() noexcept;

        /**
         * @brief Destroy the element now, don't way the next frame. 
         * WARNING : if element his destroy during update of system, it can create conflict or segfault. 
         * Use this function only if you are sur that the object don't depending af the current frame
         * 
         */
        virtual 
        void destroyImmediate() noexcept;

        /**
         * @brief add specific entity to the graph with arg to construct it and return his id
         * 
         * @tparam T 
         * @tparam Args 
         * @param args 
         * @param dependenceEntity&
         * @return GameObject&
         */
        template<typename T, typename ...Args>
        GameObject& addChild(Args&&... args) noexcept;

        bool 		operator==		(GameObject const& other);

        template <typename T>
        std::vector<T*> getComponents();

        std::list<std::unique_ptr<Component>>& getComponents () noexcept;
        const std::list<std::unique_ptr<Component>>& getComponents () const noexcept;

        std::string getRelativePath();
        
        void setTag(const std::string& newTag);
        std::string& getTag();

        bool compareTag(const std::string& toCompare);
    };
} // namespace Engine::Intermediate

#endif //_GAME_OBJECT_H