#pragma once

#include "RapidXML/rapidxml.hpp"

#include "Engine/Serialization/FieldInfo.hpp"
#include "Engine/Serialization/xml/xmlLoader.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"

class XmlManager
{
public:
    using Node = rapidxml::xml_node<>;

protected:
    rapidxml::xml_document<> doc;

    XmlSaver  saver;
    XmlLoader loader;

public:
    XmlManager() : saver(doc), loader(doc)
    {
    }

    inline Node* getRoot();

    /**
     * @brief Display the content of the xml file.
     */
    inline void print();

    /**
     * @brief Save the data into parentNode.
     * @tparam T The type of the data you want to save.
     * @param parentNode The node in which the new node will be attached to.
     * @param data The data you want to save.
     * @param fieldInfo The information about the data you want to save.
     */
    template <typename T>
    void save(Node* parentNode, const T& data, const FieldInfo& fieldInfo);

    /**
     * @brief Load the data from parentNode.
     * @tparam T The type of the data you want to load.
     * @param parentNode The node you want to load.
     * @param data The data you want to save.
     * @param fieldInfo The information about the data you want to load.
     */
    template <typename T>
    void load(Node* parentNode, T& data, const FieldInfo& fieldInfo);

    /**
     * @brief Asserts if the save() and load() function of a given type are well defined for the given value.
     * @tparam TESTED_TYPE The type you want to test.
     * @param testedValue The value you want to test.
     * @param info The information about the value you want to test.
     */
    template <typename TESTED_TYPE>
    static void testSerialization(const TESTED_TYPE& testedValue, const FieldInfo& info);
};

#include "xmlManager.inl"
