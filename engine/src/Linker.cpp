#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Linker.hpp>

using namespace GPE;

template <typename T>
void Linker<T>::setData(GameObject& owner)
{
    pData = owner.getComponent<T>();
}

void Linker<GameObject>::setData(GameObject& newData)
{
    pData = &newData;
}