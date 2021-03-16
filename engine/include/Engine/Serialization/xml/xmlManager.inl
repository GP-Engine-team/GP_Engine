﻿#include "xmlManager.hpp"

#include "RapidXML/rapidxml_print.hpp"
#include <iostream>

inline XmlManager::Node* XmlManager::getRoot()
{
    return &doc;
}

inline void XmlManager::print()
{
    std::cout << doc << std::endl;
}

template <typename T>
void XmlManager::save(Node* parentNode, const T& data, const FieldInfo& fieldInfo)
{
    saver.save(parentNode, data, fieldInfo);
}

template <typename T>
void XmlManager::load(Node* parentNode, T& data, const FieldInfo& fieldInfo)
{
    loader.load(parentNode, data, fieldInfo);
}

template <typename TESTED_TYPE>
void XmlManager::testSerialization(const TESTED_TYPE& testedValue, const FieldInfo& info)
{
    XmlManager manager;
    manager.save(manager.getRoot(), testedValue, info);
    TESTED_TYPE temp;
    manager.load(manager.getRoot(), temp, info);
    assert(temp == testedValue);
}
