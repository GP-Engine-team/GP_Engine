namespace GPE
{
template <typename T>
void save(XmlSaver& context, const std::list<T>& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(inspected.size()));

    size_t i = 0;
    for (const T& elem : inspected)
    {
        GPE::save(context, elem, XmlSaver::SaveInfo{std::to_string(i), "T", 0});
        i++;
    }

    context.pop();
}

template <typename T>
void save(class XmlSaver& context, const std::list<T>& inspected, const rfk::Field& info)
{
    GPE::save(context, inspected, fieldToSaveInfo(info));
}

template <typename T>
void save(class XmlSaver& context, const std::list<T*>& saved, const rfk::Field& info)
{
    GPE::save(context, saved, fieldToSaveInfo(info));
}

template <typename T>
void save(XmlSaver& context, const std::list<T*>& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(inspected.size()));

    size_t i = 0;
    for (T* elem : inspected)
    {
        if constexpr (std::is_base_of_v<rfk::Object, T>)
        {
            GPE::save(context, elem, XmlSaver::SaveInfo{std::to_string(i), "T", elem->getArchetype().id});
        }
        else
        {
            GPE::save(context, elem, XmlSaver::SaveInfo{std::to_string(i), "T", 0});
        }
        i++;
    }

    context.pop();
}

template <typename T>
void save(XmlSaver& context, const std::vector<T>& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(inspected.size()));

    size_t i = 0;
    for (const T& elem : inspected)
    {
        GPE::save(context, elem, XmlSaver::SaveInfo{std::to_string(i), "T", 0});
        i++;
    }

    context.pop();
}

template <typename T, typename U>
void save(XmlSaver& context, const std::pair<T, U>& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    GPE::save(context, inspected.first, XmlSaver::SaveInfo{"key", "unknown", 0});
    GPE::save(context, inspected.second, XmlSaver::SaveInfo{"value", "unknown", 0});

    context.pop();
}

template <typename KEY, typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(inspected.size()));

    size_t i = 0;
    for (const std::pair<const KEY, VALUE>& pair : inspected)
    {
        GPE::save(context, pair, XmlSaver::SaveInfo{std::to_string(i), "std::pair", 0});

        i++;
    }

    context.pop();
}
template <typename KEY, typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& inspected, const rfk::Field& info)
{
    GPE::save(context, inspected, XmlSaver::SaveInfo{info.name, "unknown", 0});
}

template <typename T>
void save(XmlSaver& context, const std::unique_ptr<T>& saved, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, saved.get(), info);
}

} // namespace GPE