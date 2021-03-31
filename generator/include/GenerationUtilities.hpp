#pragma once

#include <string>

namespace kodgen
{
class StructClassInfo;
class ComplexProperty;
} // namespace kodgen

std::string generateSerializationFunction(const kodgen::StructClassInfo& entity,
                                          kodgen::ComplexProperty const& property, const std::string& functionName,
                                          const std::string& argClassName, const std::string& fieldCallingFunction,
                                          std::string extraQualifier = "");