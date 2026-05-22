#include "EditorLayer.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Scene/Components.h"

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

        m_texture = Texture2D::Create("../../../Editor/assets/klauncher.png");

        m_activeScene = CreateRef<Scene>();
        m_square = m_activeScene->CreateEntity("sQUARE");
        m_square.AddComponent<SpriteRendererComponent>(glm::vec4{0.1f, 1.0f, 0.2f, 1.0f});
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        if (m_viewportFocused)
            m_cameraController.OnUpdate(ts);

        if (FramebufferSpecification spec = m_framebuffer->GetSpecification();
            m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
            (spec.width != m_viewportSize.x || spec.height != m_viewportSize.y ))
        {
            // we rescale the framebuffer to the actual window size here and reset the glViewport
            m_framebuffer->Resize(m_viewportSize.x, m_viewportSize.y);
            m_cameraController.OnResize(m_viewportSize.x, m_viewportSize.y);
        }


        Renderer2D::ResetStats();
        m_framebuffer->Bind();
        // Clean viewport's framebuffer
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
        RenderCommand::Clear();

        Renderer2D::BeginScene(m_cameraController.GetCamera());
        m_activeScene->OnUpdate(ts);

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

            if (ImGui::BeginMenu("Help"))
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
            m_viewportFocused = ImGui::IsWindowFocused();
            m_viewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportHovered);

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
        ImGui::End();
        ImGui::PopStyleVar(); // Viewport

        if (ImGui::Begin("Properties"))
        {
            if (m_square)
            {
                auto& tag = m_square.GetComponent<TagComponent>().tag;
                ImGui::Text("%s", tag.c_str());
                auto& color = m_square.GetComponent<SpriteRendererComponent>().color;
                ImGui::ColorEdit4("Color", glm::value_ptr(color));
            }
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
