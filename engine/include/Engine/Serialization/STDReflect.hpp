#pragma once

#include <string>
/* #include <Refureku/Object.h> */
#include "Refureku/TypeInfo/Archetypes/Class.h"
#include <vector>
#include <list>

template<typename T>
class Vector : public std::vector<T>
{
public:
    static rfk::Class const& staticGetArchetype();    

};

// Vector metadata
template <typename T>
rfk::Class const& Vector<T>::staticGetArchetype()
{
    static rfk::Class type("Vector", std::hash<std::string>()("Vector"),
                           sizeof(Vector<T>));

    return type;
}

// Register the class at file level
template <typename T>
rfk::ArchetypeRegisterer VectorRegisterer = &Vector<T>::staticGetArchetype();



template <typename T>
class List : public std::list<T>
{
public:
    static rfk::Class const& staticGetArchetype();
};

// List metadata
template <typename T>
rfk::Class const& List<T>::staticGetArchetype()
{
    static rfk::Class type("List", std::hash<std::string>()("List"), sizeof(List<T>));

    return type;
}


class MyCustomClass 
{
public:
    static rfk::Class const& staticGetArchetype()
    {
        static rfk::Class type("MyCustomClass", std::hash<std::string>()("MyCustomClass"), sizeof(MyCustomClass));

        return type;
    }
};

// Register the class at file level
//template<typename T>
//rfk::ArchetypeRegisterer ListRegisterer = &List<T>::staticGetArchetype();

namespace GPE
{
class Component;
}
//
//template<typename T>
//rfk::ArchetypeRegisterer ListRegisterer = &List<T*>::staticGetArchetype();
