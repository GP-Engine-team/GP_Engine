#include <imgui.h>

namespace GPE
{
template <typename T>
typename std::enable_if<std::is_base_of<rfk::Object, T>::value>::type save(class XmlSaver&                context,
                                                                           const std::list<T*>& saved,
                                                                           const rfk::Field&              info)
{
    context.push(info);
    context.appendAttribute(context.top(), "size", std::to_string(saved.size()));

    size_t i = 0;
    for (rfk::Object* elem : saved)
    {
        GPE::save(context, elem, XmlSaver::SaveInfo{"elem" + i, "Object*", 0});
        i++;
    }

    context.pop();
}

//template <typename T>
//typename std::enable_if<std::is_base_of<rfk::Object, T>::value>::type save(class XmlSaver&      context,
//                                                                           const List<T*>& saved,
//                                                                           const rfk::Field&    info)
//{
//    GPE::save(context, static_cast<const std::list<T*>&>(saved), info);
//}

//template <>
//void save(XmlSaver& context, rfk::Object* const& inspected, const rfk::Field& info)
//{
//}

//template <>
//void save(class XmlSaver& context, rfk::Object* const& inspected, const XmlSaver::SaveInfo& info)
//{
//
//}

template <typename T>
void load(XmlLoader& context, std::list<T*>& inspected, const rfk::Field& info)
{
}

//template <typename T>
//void load(XmlLoader& context, List<T*>& inspected, const rfk::Field& info)
//{
//}

} // namespace GPE