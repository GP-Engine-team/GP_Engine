#include "Editor/SceneEditor.hpp"
#include "imgui/imgui.h"

namespace Editor
{

SceneEditor::SceneEditor(GPE::Scene& scene)
	: sceneViewer{scene}
{

}


void SceneEditor::render() const
{
	sceneViewer.render();
}

} // End of namespace Editor
