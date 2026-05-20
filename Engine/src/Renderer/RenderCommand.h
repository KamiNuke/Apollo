#pragma once
#include "RendererAPI.h"

namespace Apollo
{
    class RenderCommand
    {
    public:
        inline static void Init()
        {
            s_rendererAPI->Init();
        }

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

        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0)
        {
            s_rendererAPI->DrawIndexed(vertexArray, indexCount);
        }

    private:
        static Scope<RendererAPI> s_rendererAPI;
    };
} // Apollo
