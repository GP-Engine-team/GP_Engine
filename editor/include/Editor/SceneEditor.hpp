﻿#pragma once

#include "Engine/Intermediate/Viewers/SceneViewer.hpp"
#include "Editor/Inspectors/Inspector.hpp"

namespace Editor
{

class SceneEditor
{
public:
	//GPE::SceneViewer sceneViewer;
	//Inspector		 inspector;

	SceneEditor(const GPE::Scene& scene);

	void render() const;
};

}
