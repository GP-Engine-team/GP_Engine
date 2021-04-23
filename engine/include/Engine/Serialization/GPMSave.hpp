#pragma once

#include "xml/xmlSaver.hpp"

namespace GPM
{
struct Transform;
struct SplitTransform;
union Vector3;
union Quaternion;
}

namespace GPE
{
/**
 * @brief Specialization for SplitTransform data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const GPM::SplitTransform& data, const rfk::Field& info);

/**
 * @brief Specialization for Vec3 data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const GPM::Vector3& data, const XmlSaver::SaveInfo& info);

/**
 * @brief Specialization for Quaternion data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const GPM::Quaternion& data, const XmlSaver::SaveInfo& info);

/**
 * @brief Specialization for Matrix4 data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const GPM::Matrix4& data, const XmlSaver::SaveInfo& info);

} // namespace GPE