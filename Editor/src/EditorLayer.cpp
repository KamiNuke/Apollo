#include "EditorLayer.h"

#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "Scene/Components.h"
#include "Scene/SceneSerializer.h"

namespace Apollo
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
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

        m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

        /*
        m_square = m_activeScene->CreateEntity("sQUARE");
        m_square.AddComponent<SpriteRendererComponent>(glm::vec4{0.1f, 1.0f, 0.2f, 1.0f});

        m_cameraEntity = m_activeScene->CreateEntity("Camera A");
        m_cameraEntity.AddComponent<CameraComponent>();

        m_secondCamera = m_activeScene->CreateEntity("Camera B");
        auto& cc = m_secondCamera.AddComponent<CameraComponent>();
        cc.primary = false;

        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {

            }

            void OnDestroy()
            {

            }

            void OnUpdate(Timestep ts)
            {
                auto& position = GetComponent<TransformComponent>().position;
                float speed = 5.0f;

                if (Input::IsKeyPressed(APOLLO_KEY_A))
                    position.x -= speed * ts;
                if (Input::IsKeyPressed(APOLLO_KEY_D))
                    position.x += speed * ts;
                if (Input::IsKeyPressed(APOLLO_KEY_W))
                    position.y += speed * ts;
                if (Input::IsKeyPressed(APOLLO_KEY_S))
                    position.y -= speed * ts;
            }
        };

        m_cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        m_secondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
*/
        m_sceneHierarchyPanel.SetContext(m_activeScene);
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        if (FramebufferSpecification spec = m_framebuffer->GetSpecification();
            m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
            (spec.width != m_viewportSize.x || spec.height != m_viewportSize.y ))
        {
            // we rescale the framebuffer to the actual window size here and reset the glViewport
            m_framebuffer->Resize(m_viewportSize.x, m_viewportSize.y);
            m_editorCamera.SetViewportSize(m_viewportSize.x, m_viewportSize.y);
            m_activeScene->OnViewportResize(m_viewportSize.x, m_viewportSize.y);
        }

        if (m_viewportFocused)
            m_editorCamera.OnUpdate(ts);

        Renderer2D::ResetStats();
        m_framebuffer->Bind();
        // Clean viewport's framebuffer
        RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f ,1.0f});
        RenderCommand::Clear();

        m_activeScene->OnUpdateEditor(ts, m_editorCamera);
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
                if (ImGui::MenuItem("New", "CTRL+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Open", "CTRL+O"))
                {
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.countSelectionMax = 1;
                    config.flags = ImGuiFileDialogFlags_Modal;
                    ImGuiFileDialog::Instance()->OpenDialog("OpenFile", "Choose a File", ".apollo", config);
                }

                if (ImGui::MenuItem("Save As", "CTRL+SHIFT+S"))
                {
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.countSelectionMax = 1;
                    config.flags = ImGuiFileDialogFlags_Modal;
                    ImGuiFileDialog::Instance()->OpenDialog("SaveFileAs", "Save a File", ".apollo", config);
                }

                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About"))
                    m_aboutUsModal = true;

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if (m_aboutUsModal)
        {
            ImGui::OpenPopup("About");
            m_aboutUsModal = false;
        }

        //About window
        {
            // Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("About Apollo");
                if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
        }

        // File dialogue
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(600,300));

            SaveSceneAs();

            OpenScene();

            ImGui::PopStyleVar();
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
            if (!ImGui::IsAnyItemActive())
                Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportHovered && !m_viewportFocused);

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

        // Gizmos
        {
            Entity selectedEntity = m_sceneHierarchyPanel.GetSelectedEntity();
            if (selectedEntity && m_gizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();
                float windowWidth = ImGui::GetWindowWidth();
                float windowHeight = ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                // Runtime camera
                //auto cameraEntity = m_activeScene->GetPrimaryCameraEntity();
                //const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
                //const glm::mat4& cameraProjection = camera.GetProjection();
                //glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

                // Editor CAMERA
                const glm::mat4& cameraProjection = m_editorCamera.GetProjection();
                glm::mat4 cameraView = m_editorCamera.GetViewMatrix();

                auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
                glm::mat4 transform = transformComponent.GetTransform();

                bool snap = Input::IsKeyPressed(APOLLO_KEY_LCTRL);
                float snapValue = 0.5f; // snap to 0.5f for translation/scale

                if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
                    snapValue = 45.0f;

                float snapValues[3] = {snapValue, snapValue, snapValue};

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                    static_cast<ImGuizmo::OPERATION>(m_gizmoType), ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
                    nullptr, snap ? snapValues : nullptr);

                if (ImGuizmo::IsUsing())
                {
                    glm::vec3 translation, rotation, scale;
                    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));
                    transformComponent.position = translation;
                    transformComponent.rotation = glm::radians(rotation);
                    transformComponent.scale = scale;

                    // Do i even need it?
                    //ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));

                }
            }

        }

        ImGui::End();
        ImGui::PopStyleVar(); // Viewport

        m_sceneHierarchyPanel.OnImGuiRender();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_editorCamera.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.GetRepeat() > 0)
            return false;

        bool control = Input::IsKeyPressed(APOLLO_KEY_LCTRL) || Input::IsKeyPressed(APOLLO_KEY_RCTRL);
        bool shift = Input::IsKeyPressed(APOLLO_KEY_LSHIFT) || Input::IsKeyPressed(APOLLO_KEY_RSHIFT);
        switch (e.GetKeyCode())
        {
            case APOLLO_KEY_S:
            {
                if (control && shift)
                {
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.countSelectionMax = 1;
                    config.flags = ImGuiFileDialogFlags_Modal;
                    ImGuiFileDialog::Instance()->OpenDialog("SaveFileAs", "Save a File", ".apollo", config);
                }

                break;
            }

            case APOLLO_KEY_N:
            {
                if (control)
                {
                    NewScene();
                }

                break;
            }

            case APOLLO_KEY_O:
            {
                if (control)
                {
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.countSelectionMax = 1;
                    config.flags = ImGuiFileDialogFlags_Modal;
                    ImGuiFileDialog::Instance()->OpenDialog("OpenFile", "Choose a File", ".apollo", config);
                }

                break;
            }

            //Gizmo
            case APOLLO_KEY_Q:
            {
                if (!ImGuizmo::IsUsing())
                    m_gizmoType = -1;
                break;
            }
            case APOLLO_KEY_W:
            {
                if (!ImGuizmo::IsUsing())
                    m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case APOLLO_KEY_E:
            {
                if (!ImGuizmo::IsUsing())
                    m_gizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case APOLLO_KEY_R:
            {
                if (!ImGuizmo::IsUsing())
                    m_gizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            }

            default:
                break;
        }

        return true;
    }

    void EditorLayer::NewScene()
    {
        m_activeScene = CreateRef<Scene>();
        m_activeScene->OnViewportResize(m_viewportSize.x, m_viewportSize.y);
        m_sceneHierarchyPanel.SetContext(m_activeScene);
    }

    void EditorLayer::OpenScene()
    {
        if (ImGuiFileDialog::Instance()->Display("OpenFile")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

                m_activeScene = CreateRef<Scene>();
                m_activeScene->OnViewportResize(m_viewportSize.x, m_viewportSize.y);
                m_sceneHierarchyPanel.SetContext(m_activeScene);

                SceneSerializer serializer(m_activeScene);
                serializer.Deserialize(filePathName);
            }
            // close
            ImGuiFileDialog::Instance()->Close();
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        if (ImGuiFileDialog::Instance()->Display("SaveFileAs")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                SceneSerializer serializer(m_activeScene);
                serializer.Serialize(filePathName);
            }
            // close
            ImGuiFileDialog::Instance()->Close();
        }
    }
}
