namespace GPE
{
template <typename T>
void save(XmlSaver& context, const std::list<T>& saved, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(saved.size()));

    size_t i = 0;
    for (const T& elem : saved)
    {
        GPE::save(context, elem, XmlSaver::SaveInfo{std::to_string(i), "T", 0});
        i++;
    }

    context.pop();
}

template <typename T>
void save(class XmlSaver& context, const std::list<T>& saved, const rfk::Field& info)
{
    GPE::save(context, saved, fieldToSaveInfo(info));
}

template <typename T>
void save(class XmlSaver& context, const std::list<T*>& saved, const rfk::Field& info)
{
    GPE::save(context, saved, fieldToSaveInfo(info));
}

template <typename T>
void save(XmlSaver& context, const std::list<T*>& saved, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(saved.size()));

    size_t i = 0;
    for (T* elem : saved)
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
void save(XmlSaver& context, const std::vector<T>& saved, const rfk::Field& info)
{
    GPE::save(context, saved, fieldToSaveInfo(info));
}

template <typename T>
void save(XmlSaver& context, const std::vector<T>& saved, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(saved.size()));

    size_t i = 0;
    for (const T& elem : saved)
    {
        GPE::save(context, elem, XmlSaver::SaveInfo{std::to_string(i), "T", 0});
        i++;
    }

    context.pop();
}

template <typename T, typename U>
void save(XmlSaver& context, const std::pair<T, U>& saved, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    GPE::save(context, saved.first, XmlSaver::SaveInfo{"key", "unknown", 0});
    GPE::save(context, saved.second, XmlSaver::SaveInfo{"value", "unknown", 0});

    context.pop();
}

template <typename KEY, typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& saved, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(saved.size()));

    size_t i = 0;
    for (const std::pair<const KEY, VALUE>& pair : saved)
    {
        GPE::save(context, pair.first, XmlSaver::SaveInfo{"key" + std::to_string(i), "unknown", 0});
        GPE::save(context, pair.second, XmlSaver::SaveInfo{"value" + std::to_string(i), "unknown", 0});

        i++;
    }

    context.pop();
}
template <typename KEY, typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& saved, const rfk::Field& info)
{
    GPE::save(context, saved, XmlSaver::SaveInfo{info.name, "unknown", 0});
}

template <typename T>
void save(XmlSaver& context, const std::unique_ptr<T>& saved, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, saved.get(), info);
}

} // namespace GPE