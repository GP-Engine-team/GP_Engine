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
void save(XmlSaver& context, const std::string& saved, const rfk::Field& info)
{
    context.saveAsString(saved, info);
}

template <>
void save(XmlSaver& context, const std::string& saved, const XmlSaver::SaveInfo& info)
{
    context.saveAsString(saved, info);
}

template <>
void save(XmlSaver& context, const int& saved, const rfk::Field& info)
{
    context.saveAsString(std::to_string(saved), info);
}

template <>
void save(XmlSaver& context, const int& saved, const XmlSaver::SaveInfo& info)
{
    context.saveAsString(std::to_string(saved), info);
}

template <>
void save(XmlSaver& context, const unsigned int& saved, const rfk::Field& info)
{
    context.saveAsString(std::to_string(saved), info);
}

template <>
void save(XmlSaver& context, const size_t& saved, const XmlSaver::SaveInfo& info)
{
    context.saveAsString(std::to_string(saved), info);
}

template <>
void save(XmlSaver& context, const char& saved, const rfk::Field& info)
{
    context.saveAsString(std::string(1, saved), info);
}

template <>
void save(XmlSaver& context, const float& saved, const rfk::Field& info)
{
    context.saveAsString(std::to_string(saved), info);
}

template <>
void save(XmlSaver& context, const bool& saved, const rfk::Field& info)
{
    context.saveAsString(saved ? "true" : "false", info);
}

template <>
void save(XmlSaver& context, const bool& saved, const XmlSaver::SaveInfo& info)
{
    context.saveAsString(saved ? "true" : "false", info);
}

template <>
void save(XmlSaver& context, rfk::Object* const & saved, const rfk::Field& info)
{
    auto i = reinterpret_cast<std::uintptr_t>(saved);
    context.saveAsString(std::to_string(i), info);
}

template <>
void save(XmlSaver& context, rfk::Object* const& saved, const XmlSaver::SaveInfo& info)
{
    auto i = reinterpret_cast<std::uintptr_t>(saved);
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