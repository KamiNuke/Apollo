#pragma once
#include <Apollo.h>
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
        OrthographicCameraController m_cameraController;

        Ref<Framebuffer> m_framebuffer;
        glm::vec2 m_viewportSize = {0.0f, 0.0f};
        bool m_viewportFocused = false;
        bool m_viewportHovered = false;

        //temp
        Ref<Scene> m_activeScene;
        Entity m_square;
        Entity m_cameraEntity;
        Entity m_secondCamera;

        bool m_primaryCamera = true;

        Apollo::Ref<Apollo::Shader> m_flatColorShader;
        Apollo::Ref<Apollo::VertexArray> m_squareVA;
        Apollo::Ref<Apollo::Texture2D> m_texture;
        glm::vec3 m_texturePosition {0.0f, 0.5f, -0.1f};
        glm::vec4 m_squareColor {0.9f, 0.2f, 0.4f, 1.0f};

        // Panels
        SceneHierarchyPanel m_sceneHierarchyPanel;
    };
}
