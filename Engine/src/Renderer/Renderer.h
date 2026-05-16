#pragma once
#include <memory>

#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Apollo
{
    class Renderer
    {
    public:

        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

        [[nodiscard]] static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };
} // Apollo
