/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

template <typename T>
void Type::fillType(Type& out_type) noexcept
{
	TypePart& currPart = out_type.parts.emplace_back(TypePart{ 0u, ETypePartDescriptor::Undefined, 0u });

	//Const
	if constexpr (std::is_const_v<T>)
	{
		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::Const;
	}

	//Volatile
	if constexpr (std::is_volatile_v<T>)
	{
		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::Volatile;
	}

	if constexpr (std::is_array_v<T>)
	{
		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::CArray;
		currPart.additionalData = static_cast<decltype(currPart.additionalData)>(std::extent_v<T>);

		fillType<std::remove_extent_t<T>>(out_type);
	}
	else if constexpr (std::is_pointer_v<T>)
	{
		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::Ptr;
		fillType<std::remove_pointer_t<T>>(out_type);
	}
	else if constexpr (std::is_lvalue_reference_v<T>)
	{
		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::LRef;
		fillType<std::remove_reference_t<T>>(out_type);
	}
	else if constexpr (std::is_rvalue_reference_v<T>)
	{
		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::RRef;
		fillType<std::remove_reference_t<T>>(out_type);
	}
	else
	{
		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::Value;

		out_type.archetype = rfk::getArchetype<std::decay_t<T>>();
	}
}

template <typename T>
Type const& Type::getType() noexcept
{
	static Type	result;
	static bool	initialized = false;
	
	if (!initialized)
	{
		initialized = true;
		fillType<T>(result);
	}

	return result;
}

template <typename... ArgTypes>
inline Type& Type::addPart(ArgTypes&&... args) noexcept
{
	parts.emplace_back(std::forward<ArgTypes>(args)...);

	return *this;
}

inline Type& Type::addPart(TypePart const& newPart) noexcept
{
	parts.push_back(newPart);

	return *this;
}

inline Type& Type::removePart() noexcept
{
	if (!parts.empty())
	{
		parts.erase(parts.cbegin());
	}

	return *this;
}

inline bool Type::isPointer() const noexcept
{
	return !parts.empty() && parts.front().isPointer();
}

inline bool Type::isLValueReference() const	noexcept
{
	return !parts.empty() && parts.front().isLValueReference();
}

inline bool Type::isRValueReference() const	noexcept
{
	return !parts.empty() && parts.front().isRValueReference();
}

inline bool Type::isCArray() const noexcept
{
	return !parts.empty() && parts.front().isCArray();
}

inline bool Type::isValue() const noexcept
{
	return !parts.empty() && parts.front().isValue();
}

inline bool Type::isConst() const noexcept
{
	return !parts.empty() && parts.front().isConst();
}

inline bool Type::isVolatile() const noexcept
{
	return !parts.empty() && parts.front().isVolatile();
}

inline uint32 Type::getArraySize() const noexcept
{
	return (!parts.empty()) ? parts.front().getArraySize() : 0u;
}