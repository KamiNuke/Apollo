#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Apollo
{
    struct Renderer2DData
    {
        Ref<VertexArray> quadVertexArray;
        Ref<VertexBuffer> quadVertexBuffer;
        Ref<Texture2D> whiteTexture;

        Ref<Shader> textureShader;
    };

    static Renderer2DData* s_data;

    void Renderer2D::Init()
    {
        s_data = new Renderer2DData();

        s_data->quadVertexArray = VertexArray::Create();

        float vertices2[5*4]
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         };

        unsigned int indices2[6]
        {
            0, 1, 2,
            2, 3, 0,
        };

        s_data->quadVertexArray->Bind();

        const Ref<VertexBuffer> m_squareVB = VertexBuffer::Create(vertices2, sizeof(vertices2));
        const Ref<IndexBuffer> m_squareVI = IndexBuffer::Create(
            indices2, sizeof(indices2) / sizeof(uint32_t));

        m_squareVB->SetLayout({
            { ShaderDataType::Float3, "aPos"},
            { ShaderDataType::Float2, "aTexCoord"},
        });

        s_data->quadVertexArray->AddVertexBuffer(m_squareVB);
        s_data->quadVertexArray->SetIndexBuffer(m_squareVI);

        uint32_t whiteTextureData = 0xffffffff;
        s_data->whiteTexture = Texture2D::Create(1, 1, &whiteTextureData, sizeof(uint32_t));

        s_data->textureShader = Shader::Create("../../../Sandbox/shaders/Texture.glsl");
        s_data->textureShader->Bind();
        s_data->textureShader->SetInt("uTexture", 0);
    }

    void Renderer2D::Shutdown()
    {
        delete s_data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_data->textureShader->Bind();
        s_data->textureShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());
    }


    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f,}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        s_data->textureShader->SetFloat4("uColor", color);
        s_data->textureShader->SetFloat("uTilingFactor", 1.0f);
        s_data->whiteTexture->Bind();

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_data->textureShader->SetMat4("uTransform", transform);

        s_data->quadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->quadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        s_data->textureShader->SetFloat4("uColor", tintColor);
        s_data->textureShader->SetFloat("uTilingFactor", tilingFactor);
        texture->Bind();

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_data->textureShader->SetMat4("uTransform", transform);

        s_data->quadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->quadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
        const glm::vec4& color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
        const glm::vec4& color)
    {
        s_data->textureShader->SetFloat4("uColor", color);
        s_data->textureShader->SetFloat("uTilingFactor", 1.0f);
        s_data->whiteTexture->Bind();

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        s_data->textureShader->SetMat4("uTransform", transform);

        s_data->quadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->quadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
        const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
        const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        s_data->textureShader->SetFloat4("uColor", tintColor);
        s_data->textureShader->SetFloat("uTilingFactor", tilingFactor);
        texture->Bind();

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        s_data->textureShader->SetMat4("uTransform", transform);

        s_data->quadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->quadVertexArray);
    }
} // Apollo