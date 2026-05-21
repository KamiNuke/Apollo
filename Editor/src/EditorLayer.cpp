#include "EditorLayer.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace Apollo
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_cameraController(1280.0f / 720.0f)
    {
    }

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::OnAttach()
    {
        FramebufferSpecification fbSpec;
        fbSpec.width = 1280;
        fbSpec.height = 720;
        m_framebuffer = Framebuffer::Create(fbSpec);

        m_texture = Apollo::Texture2D::Create("../../../Editor/assets/klauncher.png");
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        m_cameraController.OnUpdate(ts);

        if (FramebufferSpecification spec = m_framebuffer->GetSpecification();
            m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
            (spec.width != m_viewportSize.x || spec.height != m_viewportSize.y ))
        {
            // we rescale the framebuffer to the actual window size here and reset the glViewport
            m_framebuffer->Resize(m_viewportSize.x, m_viewportSize.y);
            m_cameraController.OnResize(m_viewportSize.x, m_viewportSize.y);
            //RenderCommand::SetViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
        }

        m_framebuffer->Bind();

        Renderer2D::ResetStats();

        // Clean viewport's framebuffer
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
        RenderCommand::Clear();

        Renderer2D::BeginScene(m_cameraController.GetCamera());

        static float rotation = 0.0f;
        rotation += ts * 20.0f;

        Apollo::Renderer2D::DrawRotatedQuad({m_texturePosition.x, m_texturePosition.y + 0.5f, -0.1}, {1.0f, 1.0f}, rotation, m_texture);
        Apollo::Renderer2D::DrawQuad({m_texturePosition.x - 0.5f, m_texturePosition.y, -0.1}, {1.0f, 1.0f}, m_texture, 1.0f, m_squareColor);
        Apollo::Renderer2D::DrawQuad({0.0f, 1.0f}, {1.0f, 0.30f}, m_squareColor);
        Apollo::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Apollo::Renderer2D::DrawRotatedQuad({-1.0f, 0.0f}, {0.50f, 1.0f},rotation,  m_squareColor);

        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 1.0f, 0.75f};
                Apollo::Renderer2D::DrawQuad({x, y}, {0.45f, 0.45f}, color);
            }
        }
        Renderer2D::EndScene();
        m_framebuffer->Unbind();

        //Clear main FBO
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
        RenderCommand::Clear();
    }

    void EditorLayer::OnImGuiRender()
    {
        const ImGuiID dockspace_id = ImGui::GetID("Editor Dockspace");
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Create settings
        if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
        {
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
            ImGuiID dock_id_left = 0;
            ImGuiID dock_id_main = dockspace_id;
            ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_main);
            ImGuiID dock_id_left_top = 0;
            ImGuiID dock_id_left_bottom = 0;
            ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.50f, &dock_id_left_top, &dock_id_left_bottom);
            ImGui::DockBuilderDockWindow("Viewport", dock_id_main);
            ImGui::DockBuilderDockWindow("Properties", dock_id_left_top);
            ImGui::DockBuilderDockWindow("Scene", dock_id_left_bottom);
            ImGui::DockBuilderFinish(dockspace_id);
        }

        // Submit dockspace
        ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (ImGui::Begin("Statistics"))
        {
            const auto& stats = Renderer2D::GetStats();
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.drawCalls);
            ImGui::Text("Quads: %d", stats.quadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        }
        ImGui::End(); // Statistics


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f, 0.0f});
        if (ImGui::Begin("Viewport"))
        {
            // we access the ImGui window size
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            ImVec2 pos = ImGui::GetCursorScreenPos();

            ImGui::GetWindowDrawList()->AddImage(
                m_framebuffer->GetColorAttachmentRendererID(),
                ImVec2(pos.x, pos.y),
                ImVec2(pos.x + m_viewportSize.x, pos.y + m_viewportSize.y),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }
        ImGui::End(); // Viewport
        ImGui::PopStyleVar();

        if (ImGui::Begin("Properties"))
        {

        }
        ImGui::End(); // Properties

        if (ImGui::Begin("Scene"))
        {

        }
        ImGui::End(); // Scene
        //static bool showDemo = true;
        //ImGui::ShowDemoWindow(&showDemo);
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_cameraController.OnEvent(event);
    }

}