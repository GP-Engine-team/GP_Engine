#pragma once

#include "xml/xmlLoader.hpp"

namespace GPM
{
struct Transform;
struct SplitTransform;
union alignas(16) Matrix4;
union Vector3;
union Vector4;
union Quaternion;
} // namespace GPM

namespace GPE
{
/**
 * @brief Specialization for SplitTransform data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, GPM::SplitTransform& data, const rfk::Field& info);

/**
 * @brief Specialization for Vec3 data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, GPM::Vector3& data, const XmlLoader::LoadInfo& info);

/**
 * @brief Specialization for Vec4 data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, GPM::Vector4& data, const XmlLoader::LoadInfo& info);

/**
 * @brief Specialization for Quaternion data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, GPM::Quaternion& data, const XmlLoader::LoadInfo& info);

/**
 * @brief Specialization for Matrix data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, GPM::Matrix4& data, const XmlLoader::LoadInfo& info);
template <>
void load(XmlLoader& context, GPM::Matrix4& data, const rfk::Field& info);
} // namespace GPE