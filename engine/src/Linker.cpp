#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>

using namespace GPE;

template <typename T>
void Linker<T>::setData(GameObject& owner)
{
    pData = owner.getComponent<T>();
}

//template <typename T>
//void Linker<T>::save(XmlSaver& context) const
//{
//    std::string str = "";
//    if (pData)
//    {
//        str = pData->getOwner().getAbsolutePath();
//        str.erase(0, str.find_first_of('/', 0) + 1); // remove the world
//    }
//
//    GPE::save(context, str, XmlSaver::SaveInfo{"CLinker", "CLinker", 0});
//}

void Linker<GameObject>::setData(GameObject& newData)
{
    pData = &newData;
}
//
//void Linker<GameObject>::save(XmlSaver& context) const
//{
//    std::string str = "";
//    if (pData)
//    {
//        str = pData->getAbsolutePath();
//        str.erase(0, str.find_first_of('/', 0) + 1); // remove the world
//    }
//
//    GPE::save(context, str, XmlSaver::SaveInfo{"GLinker", "GLinker", 0});
//}