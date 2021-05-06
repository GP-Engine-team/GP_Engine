#pragma once

#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

namespace GPE
{
class IInspectable;
}

namespace Editor
{
class Editor;

class SceneEditor
{
protected:
    Editor* m_pEditorContext;

public:
    GPE::SceneViewer view;

private:
    void captureInputs(bool toggle);
    void checkCursor(GPE::IInspectable*& inspectedObject);

public:
    SceneEditor(Editor& editorContext, GPE::Scene& scene);

    void update(double dt);
    void render(GPE::IInspectable*& inspectedObject);
};

} // namespace Editor
