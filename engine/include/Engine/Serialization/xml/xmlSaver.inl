namespace GPE
{

template <typename T>
void save(XmlSaver& context, const T& inspected, const rfk::Field& info)
{
    GPE::save(context, inspected, fieldToSaveInfo(info));
}

}