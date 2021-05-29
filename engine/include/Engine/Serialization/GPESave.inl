namespace GPE
{

template <typename T>
void save(XmlSaver& context, const Linker<T>& data, const rfk::Field& info)
{
    GPE::save(context, data, fieldToSaveInfo(info));
}

// Linker of component
template <typename T>
void save(XmlSaver& context, const Linker<T>& data, const XmlSaver::SaveInfo& info)
{
    std::string str = "";
    if (data.pData)
    {
        str = data.pData->getOwner().getAbsolutePath();
        str.erase(0, str.find_first_of('/', 0) + 1); // remove the world
    }

    GPE::save(context, str, XmlSaver::SaveInfo{info.name, info.typeName, 0});
}

} // namespace GPE