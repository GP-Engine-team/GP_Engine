#pragma once

#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

namespace GPE
{
class IInspectable;
}

namespace Editor
{

class SceneEditor
{

public:
	GPE::SceneViewer view;

private:
	void checkCursor(GPE::IInspectable*& inspectedObject);

public:
	SceneEditor		(GPE::Scene& scene);

	void render		(GPE::IInspectable*& inspectedObject);
};

}
