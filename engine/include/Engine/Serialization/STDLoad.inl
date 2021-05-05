namespace GPE
{

template <typename T>
void load(XmlLoader& context, std::list<T>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        size_t size = std::stoll(findAttribValue(context.top(), "size"));

        inspected.clear();

        for (size_t i = 0; i < size; i++)
        {
            inspected.emplace_back();
            GPE::load(context, inspected.back(), XmlLoader::LoadInfo{std::to_string(i), "T", info.typeId});
        }

        context.pop();
    }
}

template <typename T>
void load(class XmlLoader& context, std::list<T>& inspected, const rfk::Field& info)
{
    GPE::load(context, inspected, fieldToLoadInfo(info));
}

template <typename T>
void load(XmlLoader& context, std::list<T*>& loaded, const rfk::Field& info)
{
    GPE::load(context, loaded, fieldToLoadInfo(info));
}

template <typename T>
void load(XmlLoader& context, std::list<T*>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        size_t size = std::stoll(findAttribValue(context.top(), "size"));

        for (T* elem : inspected)
        {
            // delete elem;
            elem->destroy();
        }

        inspected.clear();

        for (size_t i = 0; i < size; i++)
        {
            // T* elem = new T(); // TODO : Share with context
            // GPE::load(context, *elem, XmlSaver::SaveInfo{std::to_string(i), "T", 0});

            T* elem;
            GPE::load(context, elem, XmlLoader::LoadInfo{std::to_string(i), "T", info.typeId});
            inspected.emplace_back(elem);
        }

        context.pop();
    }
}

template <typename T>
void load(XmlLoader& context, std::vector<T>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        size_t size = std::stoll(findAttribValue(context.top(), "size"));

        inspected.clear();
        inspected.reserve(size);

        for (size_t i = 0; i < size; i++)
        {
            inspected.emplace_back();
            GPE::load(context, inspected.back(), XmlLoader::LoadInfo{std::to_string(i), "T", info.typeId});
        }

        context.pop();
    }
}

template <typename T, typename U>
void load(XmlLoader& context, std::pair<T, U>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        GPE::load(context, inspected.first, XmlLoader::LoadInfo{"key", "unknown", 0});
        GPE::load(context, inspected.second, XmlLoader::LoadInfo{"value", "unknown", 0});

        context.pop();
    }
}

template <typename KEY, typename VALUE>
void load(XmlLoader& context, std::unordered_map<KEY, VALUE>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        size_t size = std::stoll(findAttribValue(context.top(), "size"));

        inspected.clear();
        inspected.reserve(size);

        for (size_t i = 0; i < size; i++)
        {
            std::pair<KEY, VALUE> pair;
            //GPE::load(context, pair, XmlLoader::LoadInfo{std::to_string(i), "std::pair", info.typeId});
            // Load Key
            if (context.goToSubChild(info))
            {
                GPE::load(context, pair.first, XmlLoader::LoadInfo{"key", "unknown", 0});
                auto insertReturned = inspected.insert(pair);
                GPE::load(context, insertReturned.first->second, XmlLoader::LoadInfo{"value", "unknown", 0});

                context.pop();
            }
        }

        context.pop();
    }
}
template <typename KEY, typename VALUE>
void load(XmlLoader& context, std::unordered_map<KEY, VALUE>& inspected, const rfk::Field& info)
{
    GPE::load(context, inspected, XmlLoader::LoadInfo{info.name, "unknown", 0});
}

template <typename T>
void load(XmlLoader& context, std::unique_ptr<T>& loaded, const XmlLoader::LoadInfo& info)
{
    GPE::load(context, loaded.get(), info);
}

} // namespace GPE