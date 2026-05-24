#pragma once
#include <Apollo.h>

#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Apollo
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        ~EditorLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;
    private:
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);

        void NewScene();
        void OpenScene();
        void OpenScene(const std::filesystem::path& path);
        void SaveSceneAs();
    private:
        Ref<Framebuffer> m_framebuffer;
        glm::vec2 m_viewportSize = {0.0f, 0.0f};
        bool m_viewportFocused = false;
        bool m_viewportHovered = false;
        glm::vec2 m_viewportBounds[2];

        Ref<Scene> m_activeScene;
        Entity m_hoveredEntity;

        bool m_primaryCamera = true;

        EditorCamera m_editorCamera;

        int m_gizmoType = -1;

        // Panels
        SceneHierarchyPanel m_sceneHierarchyPanel;
        ContentBrowserPanel m_contentBrowserPanel;
        bool m_aboutUsModal = false;
    };
}
