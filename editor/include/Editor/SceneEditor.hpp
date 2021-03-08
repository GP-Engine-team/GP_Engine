#pragma once

#include "Engine/Intermediate/Viewers/SceneViewer.hpp"
#include "Editor/Inspectors/Inspector.hpp"

namespace Editor
{
	class SceneEditor
	{
	public:
		GPE::SceneViewer sceneViewer;
		Inspector* inspector;

		void render() const
		{
			sceneViewer.render();
		}

	};

}
