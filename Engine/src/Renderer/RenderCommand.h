#pragma once
#include "RendererAPI.h"

namespace Apollo
{
    class RenderCommand
    {
    public:
        inline static void SetViewport(int x, int y, int width, int height)
        {
            s_rendererAPI->SetViewport(x, y, width, height);
        }
        inline static void SetClearColor(const glm::vec4& color)
        {
            s_rendererAPI->SetClearColor(color);
        }
        inline static void Clear()
        {
            s_rendererAPI->Clear();
        }

        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
        {
            s_rendererAPI->DrawIndexed(vertexArray);
        }

    private:
        static Scope<RendererAPI> s_rendererAPI;
    };
} // Apollo
