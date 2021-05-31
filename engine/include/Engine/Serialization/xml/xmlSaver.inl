namespace GPE
{

template <typename T>
void save(XmlSaver& context, const T& inspected, const rfk::Field& info)
{
    GPE::save(context, inspected, fieldToSaveInfo(info));
}

template <typename T>
void save(XmlSaver& context, const T& inspected, const XmlSaver::SaveInfo& info)
{
    if constexpr (std::is_enum_v<T>)
    {
        GPE::save(context, *reinterpret_cast<const std::underlying_type_t<T>*>(&inspected), info);
    }
    else
    {
        context.push(info);

        inspected.save(context);

        context.pop();
    }
}

}