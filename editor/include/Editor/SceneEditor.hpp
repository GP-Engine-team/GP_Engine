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
    void captureInputs(bool toggle);
    void checkCursor(GPE::IInspectable*& inspectedObject);

public:
    SceneEditor(GPE::Scene& scene);

    void update(double dt);
    void render(GPE::IInspectable*& inspectedObject);
};

} // namespace Editor
