#pragma once

#include <string>

#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <Kodgen/Properties/Property.h>

std::string generateSerializationFunction(const kodgen::StructClassInfo& entity,
                                          kodgen::Property const& property, const std::string& functionName,
                                          const std::string& argClassName, const std::string& fieldCallingFunction,
                                          std::string extraQualifier = "");

std::string generateSerializationFunctionDecl(const kodgen::StructClassInfo& entity, kodgen::Property const& property,
                                              const std::string& functionName,
                                              const std::string& argClassName, const std::string& fieldCallingFunction,
                                              std::string extraQualifier = "");

std::string generateSerializationFunctionImpl(const kodgen::StructClassInfo& entity, kodgen::Property const& property,
                                              const std::string& functionName,
                                              const std::string& argClassName, const std::string& fieldCallingFunction,
                                              std::string extraQualifier = "");