#pragma once

#include <list>

namespace rfk
{
class Field;
}

namespace GPE
{
	template<typename T>
	void save(class XmlSaver& context, const std::list<T>& inspected, const rfk::Field& info);
}

#include "STDSave.inl"