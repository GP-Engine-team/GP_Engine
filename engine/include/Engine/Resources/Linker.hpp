/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

class XmlSaver;

namespace GPE
{
class GameObject;
class InspectContext;

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

    // void save(XmlSaver& context) const;
    // void inspect(InspectContext& context);
};

namespace DataInspector
{
template <typename T>
bool inspect(InspectContext& context, Linker<T>& inspected, const rfk::Field& info);

template <typename T>
bool inspect(InspectContext& context, Linker<T>& inspected, const char* name);

template <typename T>
void inspect(InspectContext& context, Linker<T>& inspected);
} // namespace DataInspector

/**
 * @brief For gameObject
 */
template <>
struct Linker<GameObject> : public BaseLinker
{
    GameObject* pData = nullptr;

    void setData(GameObject& newData) override;

    // void save(XmlSaver& context) const;
    void inspect(InspectContext& context);
};

template <>
bool DataInspector::inspect<Linker<GameObject>>(InspectContext& context, Linker<GameObject>& inspected,
                                                const rfk::Field& info);

template <>
bool DataInspector::inspect<Linker<GameObject>>(InspectContext& context, Linker<GameObject>& inspected,
                                                const char* name);

template <>
void DataInspector::inspect<Linker<GameObject>>(InspectContext& context, Linker<GameObject>& inspected);

template <>
bool DataInspector::inspect<GameObject>(InspectContext& context, Linker<GameObject>& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect<GameObject>(InspectContext& context, Linker<GameObject>& inspected, const char* name);

template <>
void DataInspector::inspect<GameObject>(InspectContext& context, Linker<GameObject>& inspected);

} // namespace GPE

#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>

#include <Engine/Resources/Linker.inl>