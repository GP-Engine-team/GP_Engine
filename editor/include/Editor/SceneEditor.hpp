#pragma once

#include <Engine/Intermediate/Viewers/SceneViewer.hpp>

namespace GPE
{
class IInspectable;
}

namespace Editor
{
class Editor;

class SceneEditor
{
public:
    GPE::SceneViewer view;

private:
    void captureInputs(bool toggle);
    void checkCursor(GPE::IInspectable*& inspectedObject);

private:
    class Editor* m_editorContext = nullptr;

public:
    SceneEditor(Editor& editorContext, GPE::Scene& scene);

    void render(GPE::IInspectable*& inspectedObject);
};

} // namespace Editor
