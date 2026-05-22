#pragma once

#include "Apollo.h"
#include "imgui.h"


namespace Apollo
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);

        void SetContext(const Ref<Scene>& context);

        void OnImGuiRender();
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);
    private:
        Ref<Scene> m_context;
        Entity m_selectionContext;

        friend class Scene;
    };
} // Apollo
