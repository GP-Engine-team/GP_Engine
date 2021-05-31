template <typename T>
void XmlLoader::loadPtrData(T*& data, const LoadInfo& info, void* key)
{
    if (key == nullptr)
        return;

    auto pair = alreadyLoadedPtrs.insert({key, LoadedPtr{info}});
    if (pair.second) // Has been inserted ?
    {
        if constexpr (std::is_base_of<rfk::Object, T>::value)
        {
            std::string       idStr     = findAttribValue(top(), "typeID");
            size_t            s         = (std::stoull(idStr));
            rfk::Class const* archetype = static_cast<rfk::Class const*>(rfk::Database::getEntity(s));
            assert(archetype != 0);              // Type is not complete. Try adding corresponding include in game.cpp
            data = archetype->makeInstance<T>(); // If this crashes here, try to regenerate files.
            // TODO : Call custom instantiator ? 
        }
        else
        {
            data = new T();
        }

        pair.first->second.data = data;

        assert(data != nullptr); // Type is not default constructible.

        std::stack<Node*> otherContextHierarchy;
        otherContextHierarchy.push(&doc);
        std::swap(otherContextHierarchy, hierarchy);

        LoadInfo newInfo{std::to_string(size_t(key)), info.typeName, info.typeId};
        GPE::load(*this, *data, newInfo);

        hierarchy = std::move(otherContextHierarchy);
    }
    else
    {
        data = static_cast<T*>(pair.first->second.data); // CAST
    }
}

namespace GPE
{
template <typename T>
void load(XmlLoader& context, T& inspected, const XmlLoader::LoadInfo& info)
{
    if constexpr (std::is_enum_v<T>)
    {
        GPE::load(context, *reinterpret_cast<std::underlying_type_t<T>*>(&inspected), info);
    }
    else
    {
        if (context.goToSubChild(info))
        {
            inspected.load(context);
            context.pop();
        }
    }
}

template <typename T>
void load(XmlLoader& context, T& inspected, const rfk::Field& info)
{
    GPE::load(context, inspected, fieldToLoadInfo(info));
}

template <typename T>
void load(XmlLoader& context, T*& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        std::string str;
        if (context.loadFromStr(str, info))
        {
            std::size_t p = std::stoull(str);
            

            context.loadPtrData(inspected, info, (void*)p);
        }

        context.pop();
    }
}

template <typename T>
void load(XmlLoader& context, T*& inspected, const rfk::Field& info)
{
    load(context, inspected, fieldToLoadInfo(info));
}

} // namespace GPE