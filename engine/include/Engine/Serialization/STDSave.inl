#include <imgui.h>
#include "Refureku/TypeInfo/Variables/Field.h"

namespace GPE
{
template <typename T>
void save(class XmlSaver& context, const std::list<T*>& saved, const rfk::Field& info)
{
    GPE::save(context, saved, XmlSaver::SaveInfo{info.name, "std::list<T*>", 0});
    //context.push(info);
    //context.appendAttribute(context.top(), "size", std::to_string(saved.size()));

    //size_t i = 0;
    //for (rfk::Object* elem : saved)
    //{
    //    GPE::save(context, elem, XmlSaver::SaveInfo{"elem" + i, "Object*", 0});
    //    i++;
    //}

    //context.pop();
}

template <typename T>
void load(XmlLoader& context, std::list<T*>& loaded, const rfk::Field& info)
{
    GPE::load(context, loaded, XmlLoader::LoadInfo{info.name, "std::list<T*>", 0});
}

} // namespace GPE