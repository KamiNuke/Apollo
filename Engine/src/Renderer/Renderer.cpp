#include "Renderer.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace Apollo
{
    Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        shader->SetMat4("uViewProjection", s_sceneData->viewProjectionMatrix);
        shader->SetMat4("uTransform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
} // Apollo