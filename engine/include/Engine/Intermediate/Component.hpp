/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

namespace Engine::Intermediate
{
    class GameObject;

    class Component
    {
    protected:

    GameObject& m_gameObject;
    std::string m_name;
    bool        m_isActivated {true};

    public:

    Component(GameObject &refGameObject)
    :   m_gameObject {refGameObject},
        m_name       {__FUNCTION__}
    {}

    Component(const Component &other)               = default;
    Component(Component &&other)                    = default;
    virtual ~Component()                            = default;

    Component()                                     = delete;
    Component &operator=(Component const &other)    = delete;
    Component &operator=(Component &&other)         = delete;

    GameObject &        getGameObject() noexcept { return m_gameObject; }
    const GameObject&   getGameObject() const noexcept { return m_gameObject; }

    std::string toString() const noexcept {return m_name; };

    bool isActivated () const noexcept {return m_isActivated;}
    void setActive (bool newState) noexcept { m_isActivated = newState;}

    };

} // namespace Engine::Intermediate