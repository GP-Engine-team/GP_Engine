#pragma once

#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

namespace Editor
{

class SceneEditor
{
public:
	GPE::SceneViewer sceneViewer;

	SceneEditor(GPE::Scene& scene);

	void render() const;
};

}
