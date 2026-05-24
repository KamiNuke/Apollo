#include "ImGuiLayer.h"

#include "Core/Application.h"
#include <imgui.h>
#include <SDL3/SDL_video.h>
#include <ImGuizmo.h>

namespace Apollo
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.FontDefault = io.Fonts->AddFontFromFileTTF("../../../Editor/assets/fonts/Monocraft-ttf/Monocraft.ttf", 14.0);
        io.Fonts->AddFontFromFileTTF("../../../Editor/assets/fonts/Monocraft-ttf/weights/Monocraft-Bold.ttf", 14.0);

        SetDarkThemeColors();

        Application& app = Application::Get();
        app.GetWindow().ImGuiInit();
    }

    void ImGuiLayer::OnDetach()
    {
        Application& app = Application::Get();
        app.GetWindow().ImGuiShutdown();
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_blockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            event.handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            event.handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayer::Begin()
    {
        Application& app = Application::Get();
        app.GetWindow().ImGuiBegin();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End()
    {
        ImGui::ShowDemoWindow();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        // Rendering
        ImGui::Render();
        app.GetWindow().ImGuiEnd();
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        SetStyle();
    }

    void ImGuiLayer::SetLightThemeColors()
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsLight();

        SetStyle();
    }

    void ImGuiLayer::SetStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.TabRounding = 6.f;
        style.FrameRounding = 2.f;
        style.GrabRounding = 4.f;
        style.WindowRounding = 4.f;
        style.PopupRounding = 4.f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
} // Apollo