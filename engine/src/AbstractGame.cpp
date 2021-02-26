#include "Engine/Core/Game/AbstractGame.hpp"

#include "Engine/Test/Foo.hpp"
#include "RapidXML/rapidxml_print.hpp"
#include "Refureku/TypeInfo/Entity/EntityCast.h"
#include "rapidxml.hpp"
#include <iostream>

#include <iostream>
#include <Refureku/Refureku.h>

struct A
{
    template <typename T>
    void operator()(int*, const T& saved)
    {
    }
}; 

AbstractGame::AbstractGame()
{
    //xml_document<> doc;

    Foo f;
    A b;

    int* i = nullptr;
    f.save(b, i);
}