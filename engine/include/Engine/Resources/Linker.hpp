/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

namespace GPE
{
class GameObject;

/**
 * @brief Linker allow user to link local object in editor. Thank's to it, user can associate
 * gameObject and serialize it. Warning ! : Assocation is local of the base scene. For prefab, the base scene is the
 * first gameObject. So user cannot link gameObject that is not inside the prefab.
 *
 */
struct BaseLinker
{
    virtual void setData(GameObject&) = 0;
};

/**
 * @brief For component
 * @tparam T
 */
template <typename T>
struct Linker : public BaseLinker
{
    T* pData = nullptr;

    void setData(GameObject& owner) override;
};

/**
 * @brief For gameObject
 */
template <>
struct Linker<GameObject> : public BaseLinker
{
    GameObject* pData = nullptr;

    void setData(GameObject& newData) override;
};
} // namespace GPE