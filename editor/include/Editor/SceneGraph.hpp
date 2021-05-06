#pragma once

#include "Engine/Intermediate/GameObject.hpp"

namespace GPE
{
class IInspectable;
} // namespace GPE

namespace Editor
{
class DeferedSetParent
{
    GPE::GameObject* m_movedGO     = nullptr;
    GPE::GameObject* m_newParentGO = nullptr;

public:
    void bind(GPE::GameObject& movedGO, GPE::GameObject& newParentGO);

    void tryExecute();
};

class SceneGraph
{
protected:
    DeferedSetParent deferedSetParent;
    class Editor*    m_pEditorContext = nullptr;

protected:
    void controlPreviousItem(GPE::GameObject& gameObject, GPE::IInspectable*& selectedGameObject, int idElem);

    void recursiveSceneGraphNode(GPE::GameObject& gameObject, GPE::IInspectable*& selectedGameObject, int idElem = 0);

public:
    SceneGraph(class Editor& context);

    void renderAndGetSelected(GPE::GameObject& gameObject, GPE::IInspectable*& selectedGameObject);
};
} // namespace Editor
