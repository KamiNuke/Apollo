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
        fbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::DEPTH_24_STENCIL_8 };
        m_framebuffer = Framebuffer::Create(fbSpec);

        m_activeScene = CreateRef<Scene>();

        auto cmdArgs = Application::Get().GetSpecification().cmdArgs;
        if (cmdArgs.count > 1)
        {
            auto sceneFilePath = cmdArgs.args[1];
            SceneSerializer serializer(m_activeScene);
            serializer.Deserialize(sceneFilePath);
        }

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

        Renderer2D::ResetStats();
        m_framebuffer->Bind();
        // Clean viewport's framebuffer
        RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f ,1.0f});
        RenderCommand::Clear();
        m_framebuffer->ClearAttachment(1, -1);

        switch (m_sceneState)
        {
            case SceneState::Edit:
            {
                m_editorCamera.OnUpdate(ts);

                m_activeScene->OnUpdateEditor(ts, m_editorCamera);
                break;
            }
            case SceneState::Play:
                m_activeScene->OnUpdateRuntime(ts);
            default:
                break;
        }

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_viewportBounds[0].x;
        my -= m_viewportBounds[0].y;
        glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = static_cast<int>(mx);
        int mouseY = static_cast<int>(my);
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_framebuffer->ReadPixel(1, mouseX, mouseY);
            m_hoveredEntity = pixelData == -1 ? Entity{} : Entity{ static_cast<entt::entity>(pixelData), m_activeScene.get() };
        }
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

            std::string name = "None";
            if (m_hoveredEntity)
                name = m_hoveredEntity.GetComponent<TagComponent>().tag;

            ImGui::Text("Hovered entity: %s", name.c_str());
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
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            m_viewportFocused = ImGui::IsWindowFocused();
            m_viewportHovered = ImGui::IsWindowHovered();
            if (!ImGui::IsAnyItemActive())
                Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportHovered && !m_viewportFocused);

            // we access the ImGui window size
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            ImGui::Image(m_framebuffer->GetColorAttachmentRendererID(0),
                ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                const std::filesystem::path::value_type* path = static_cast<const std::filesystem::path::value_type*>(payload->Data);
                OpenScene(path);
            }

            ImGui::EndDragDropTarget();
        }

        // Gizmos
        {
            Entity selectedEntity = m_sceneHierarchyPanel.GetSelectedEntity();
            if (selectedEntity && m_gizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_viewportBounds[1].x - m_viewportBounds[0].x,
                    m_viewportBounds[1].y - m_viewportBounds[0].y);

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
        UIToolbar();
        m_sceneHierarchyPanel.OnImGuiRender();
        m_contentBrowserPanel.OnImGuiRender();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_editorCamera.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == APOLLO_MOUSE_BUTTON_LEFT)
        {
            if (m_viewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(APOLLO_KEY_LALT))
                m_sceneHierarchyPanel.SetSelectedEntity(m_hoveredEntity);
        }
        return false;
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
            if (ImGuiFileDialog::Instance()->IsOk())
                OpenScene(ImGuiFileDialog::Instance()->GetFilePathName());

            // close
            ImGuiFileDialog::Instance()->Close();
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& path)
    {
        if (path.extension().string() != ".apollo")
        {
            APOLLO_LOGGER_WARN("Could not load {0} - not a scene file", path.filename().string());
            return;
        }

        Ref<Scene> newScene = CreateRef<Scene>();

        SceneSerializer serializer(newScene);
        if (serializer.Deserialize(path.string()))
        {
            m_activeScene = newScene;
            m_activeScene->OnViewportResize(m_viewportSize.x, m_viewportSize.y);
            m_sceneHierarchyPanel.SetContext(m_activeScene);
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

    void EditorLayer::OnScenePlay()
    {
        m_sceneState = SceneState::Play;
        m_activeScene->OnRuntimeStart();
    }

    void EditorLayer::OnSceneStop()
    {
        m_sceneState = SceneState::Edit;
        m_activeScene->OnRuntimeStop();
    }

    void EditorLayer::UIToolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 2});
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2 {0, 0});


        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        const std::string buttonLabel = m_sceneState == SceneState::Play ? "Stop" : "Play";
        const float size = ImGui::GetWindowHeight() - 4.0f;

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        if (ImGui::Button(buttonLabel.c_str(), ImVec2{ size + 10.0f, size }))
        {
            switch (m_sceneState)
            {
                case SceneState::Edit:
                    OnScenePlay();
                    break;
                case SceneState::Play:
                    OnSceneStop();
                    break;
                default:
                    break;
            }
        }

        ImGui::PopStyleVar(2);

        ImGui::End();
    }
}
