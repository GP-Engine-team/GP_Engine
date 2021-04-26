#include "Engine/Serialization/xml/xmlSaver.hpp"
#include "RapidXML/rapidxml_print.hpp"

void XmlSaver::appendAttribute(Node* node, const std::string& name, const std::string& value)
{
    node->append_attribute(doc.allocate_attribute(doc.allocate_string(name.c_str(), name.size()),
                                                  doc.allocate_string(value.c_str(), value.size()), name.size(),
                                                  value.size()));
}

void XmlSaver::saveAsString(const std::string& saved, const rfk::Field& info)
{
    push(info);
    appendAttribute(top(), "value", saved);
    pop();
}

void XmlSaver::saveAsString(const std::string& saved, const XmlSaver::SaveInfo& info)
{
    push(info);
    appendAttribute(top(), "value", saved);
    pop();
}

void XmlSaver::print()
{
    std::cout << doc << std::endl;
}

namespace GPE
{

template <>
void save(XmlSaver& context, const std::string& inspected, const rfk::Field& info)
{
    context.saveAsString(inspected, info);
}

template <>
void save(XmlSaver& context, const std::string& inspected, const XmlSaver::SaveInfo& info)
{
    context.saveAsString(inspected, info);
}

template <>
void save(XmlSaver& context, const int& inspected, const rfk::Field& info)
{
    context.saveAsString(std::to_string(inspected), info);
}

template <>
void save(XmlSaver& context, const int& inspected, const XmlSaver::SaveInfo& info)
{
    context.saveAsString(std::to_string(inspected), info);
}

template <>
void save(XmlSaver& context, const size_t& inspected, const XmlSaver::SaveInfo& info)
{
    context.saveAsString(std::to_string(inspected), info);
}

template <>
void save(XmlSaver& context, const char& inspected, const rfk::Field& info)
{
    context.saveAsString(std::string(1, inspected), info);
}

template <>
void save(XmlSaver& context, const float& inspected, const rfk::Field& info)
{
    context.saveAsString(std::to_string(inspected), info);
}

template <>
void save(XmlSaver& context, const bool& inspected, const rfk::Field& info)
{
    context.saveAsString(inspected ? "true" : "false", info);
}

template <>
void save(XmlSaver& context, const bool& inspected, const XmlSaver::SaveInfo& info)
{
    context.saveAsString(inspected ? "true" : "false", info);
}

template <>
void save(XmlSaver& context, rfk::Object* const & inspected, const rfk::Field& info)
{
    auto i = reinterpret_cast<std::uintptr_t>(inspected);
    context.saveAsString(std::to_string(i), info);
}

template <>
void save(XmlSaver& context, rfk::Object* const& inspected, const XmlSaver::SaveInfo& info)
{
    auto i = reinterpret_cast<std::uintptr_t>(inspected);
    context.saveAsString(std::to_string(i), info);
}

template <>
void save(XmlSaver& context, rfk::Method const* const& data, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    GPE::save(context, size_t(data->id), XmlSaver::SaveInfo{"id", "int", 0});

    context.pop();
}

} // namespace GPE