namespace GPE
{
template <typename T>
void load(XmlLoader& context, T& inspected, const rfk::Field& info)
{
    if (context.goToSubChild(info))
    {
        inspected.load(context);
        context.pop();
    }
}

template <typename T>
void load(XmlLoader& context, T& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        inspected.load(context);
        context.pop();
    }
}

template <typename T>
void load(XmlLoader& context, T*& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        //context.saveAsString(std::to_string(size_t(inspected)), info);
        std::string str;
        if (context.loadFromStr(str, info))
        {
            std::size_t p = std::stoll(str);
            

            context.loadPtrData(inspected, info, p);
        }

        context.pop();
    }
}

template <typename T>
void load(XmlLoader& context, T*& inspected, const rfk::Field& info)
{
    load(context, inspected, XmlLoader::LoadInfo{info.name, info.type.archetype->name, info.type.archetype->id});
}

} // namespace GPE