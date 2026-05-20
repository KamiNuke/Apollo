#pragma once
#include "glad/glad.h"
#include "Renderer/RendererAPI.h"

namespace Apollo
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        void Init() override;
        void SetViewport(int x, int y, int width, int height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
    };
} // Apollo
