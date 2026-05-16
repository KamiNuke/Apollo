#pragma once

namespace Apollo
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        virtual void SwapBuffer() = 0;
        virtual void SetViewportSize(int x, int y, int w, int h) = 0;

        virtual void ImGuiInit() = 0;
        virtual void ImGuiShutdown() = 0;
        virtual void ImGuiBegin() = 0;
        virtual void ImGuiEnd() = 0;
    };
} // Apollo
