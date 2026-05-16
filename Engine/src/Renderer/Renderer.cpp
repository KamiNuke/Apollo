#include "Renderer.h"

#include "RenderCommand.h"

namespace Apollo
{
    void Renderer::BeginScene()
    {
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
    {
        shader->Bind();
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
} // Apollo