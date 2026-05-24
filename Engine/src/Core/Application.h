#pragma once
#include <cassert>
#include <memory>

#include "Base.h"
#include "LayerStack.h"
#include "Window.h"
#include "Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"

int main(int argc, char* argv[]);

namespace Apollo
{
    struct ApplicationCommandLineArgs
    {
        int count = 0;
        char** args = nullptr;

        const char* operator[](int index) const
        {
            APOLLO_ASSERT(index < count, "overflow");
            return args[index];
        }
    };

    struct ApplicationSpecification
    {
        std::string name = "Apollo";
        std::string workingDirectory;
        ApplicationCommandLineArgs cmdArgs;
    };

    class Application
    {
    public:
        explicit Application(const ApplicationSpecification& specification);

        virtual ~Application();

        void Run();
        void Close();

        void OnEvent(Event& e);
        virtual void OnUpdate() {}
        virtual void OnRender() {}

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        [[nodiscard]] inline Window& GetWindow() const { return *m_window; }
        [[nodiscard]] ImGuiLayer* GetImGuiLayer() { return m_imGuiLayer; }

        /*
         * Basically entry function for user apps. Define in order to use
         */
        Application* CreateApplication();

        static inline Application& Get() { return *s_instance; };
        ApplicationSpecification GetSpecification() const { return m_specification; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        ApplicationSpecification m_specification;
        Scope<Window> m_window;

        ImGuiLayer* m_imGuiLayer;
        LayerStack m_layerStack;

        bool m_isRunning = true;
        bool m_isMinimized = false;

        float m_lastFrameTime = 0.0f;
    private:
        static Application* s_instance;
        friend int ::main(int argc, char** argv);
    };

    Application* CreateApplication(ApplicationCommandLineArgs args);
} // Apollo
