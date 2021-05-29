namespace GPE
{

template <typename T>
void save(XmlSaver& context, const Linker<T>& data, const rfk::Field& info)
{
    GPE::save(context, data, fieldToSaveInfo(info));
}

} // namespace GPE