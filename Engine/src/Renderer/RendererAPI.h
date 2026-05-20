#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Apollo
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0, OpenGL = 1, Vulkan = 2
        };
    public:
        virtual void Init() = 0;
        virtual ~RendererAPI() = default;

        virtual void SetViewport(int x, int y, int width, int height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

        static inline API GetAPI() { return s_API; }
        static Scope<RendererAPI> Create();
    private:
        static API s_API;
    };
} // Apollo
