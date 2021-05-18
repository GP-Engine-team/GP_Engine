#include <Engine/Core/Physics/Collisions/Collider.hpp>

// Generated
#include "Generated/Collider.rfk.h"

File_GENERATED

using namespace GPE;

Collider::~Collider() noexcept
{
	if (material && material->isReleasable())
	{
		material->release();
	}
}
