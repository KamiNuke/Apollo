#include "EditorLayer.h"

#include "imgui.h"
#include "imgui_internal.h"

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_cameraController(1280.0f / 720.0f)
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
    //m_frameBuffer = Apollo::CreateRef<Apollo::OpenGLFrameBuffer>(1280, 720);
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate(Apollo::Timestep ts)
{
    m_cameraController.OnUpdate(ts);

    Apollo::Renderer2D::ResetStats();

    Apollo::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
    Apollo::RenderCommand::Clear();

    Apollo::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
    Apollo::RenderCommand::Clear();

    Apollo::Renderer2D::BeginScene(m_cameraController.GetCamera());

    Apollo::Renderer2D::EndScene();
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
                Apollo::Application::Get().Close();

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (ImGui::Begin("Statistics"))
    {
        const auto& stats = Apollo::Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::End();
    }

    /*
    if (ImGui::Begin("Viewport"))
    {
        // we access the ImGui window size
        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;

        // we rescale the framebuffer to the actual window size here and reset the glViewport
        m_frameBuffer->Resize(window_width, window_height);
        Apollo::RenderCommand::SetViewport(0, 0, window_width, window_height);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        // and here we can add our created texture as image to ImGui
        // unfortunately we need to use the cast to void* or I didn't find another way tbh
        ImGui::GetWindowDrawList()->AddImage(
            m_frameBuffer->GetTexture(),
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x + window_width, pos.y + window_height),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
        ImGui::End();
    }
    */

    if (ImGui::Begin("Properties"))
    {
        ImGui::End();
    }

    if (ImGui::Begin("Scene"))
    {
        ImGui::End();
    }
    //static bool showDemo = true;
    //ImGui::ShowDemoWindow(&showDemo);
}

void EditorLayer::OnEvent(Apollo::Event& event)
{
    m_cameraController.OnEvent(event);
}