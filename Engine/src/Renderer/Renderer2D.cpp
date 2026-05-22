#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Apollo
{
    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        float texIndex;
        float tilingFactor;
    };

    struct Renderer2DData
    {
        static constexpr uint32_t maxQuads = 20000;
        static constexpr uint32_t maxVertices = maxQuads * 4;
        static constexpr uint32_t maxIndices = maxQuads * 6;
        static constexpr uint32_t maxTextureSlots = 32;

        Ref<VertexArray> quadVertexArray;
        Ref<VertexBuffer> quadVertexBuffer;
        Ref<Shader> quadShader;
        Ref<Texture2D> whiteTexture;

        uint32_t quadIndexCount = 0;
        QuadVertex* quadVertexBufferBase = nullptr;
        QuadVertex* quadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 1; // 0 - white texture

        glm::vec4 quadVertexPositions[4];

        Renderer2D::Statistics stats;
    };

    static Renderer2DData* s_data = nullptr;

    void Renderer2D::Init()
    {
        s_data = new Renderer2DData();
        s_data->quadVertexArray = VertexArray::Create();

        s_data->quadVertexArray->Bind();

        s_data->quadVertexBuffer = VertexBuffer::Create(s_data->maxVertices * sizeof(QuadVertex));
        s_data->quadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "aPos"},
            { ShaderDataType::Float4, "aColor"},
            { ShaderDataType::Float2, "aTexCoord"},
            { ShaderDataType::Float, "aTexIndex"},
            { ShaderDataType::Float, "aTilingFactor"},
        });
        s_data->quadVertexArray->AddVertexBuffer(s_data->quadVertexBuffer);

        s_data->quadVertexBufferBase = new QuadVertex[s_data->maxVertices];

        uint32_t* quadIndices = new uint32_t[s_data->maxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_data->maxIndices; i += 6)
        {
            quadIndices[i+0] = offset + 0;
            quadIndices[i+1] = offset + 1;
            quadIndices[i+2] = offset + 2;

            quadIndices[i+3] = offset + 2;
            quadIndices[i+4] = offset + 3;
            quadIndices[i+5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_data->maxIndices);
        s_data->quadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        uint32_t whiteTextureData = 0xffffffff;
        s_data->whiteTexture = Texture2D::Create(1, 1, &whiteTextureData, sizeof(uint32_t));

        int samplers[s_data->maxTextureSlots];
        for (uint32_t i = 0; i < s_data->maxTextureSlots; i++)
            samplers[i] = i;

        s_data->quadShader = Shader::Create("../../../Sandbox/shaders/Texture.glsl");
        s_data->quadShader->Bind();
        s_data->quadShader->SetIntArray("uTextures", samplers, s_data->maxTextureSlots);

        s_data->textureSlots[0] = s_data->whiteTexture;

        s_data->quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f };
        s_data->quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f };
        s_data->quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f };
        s_data->quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown()
    {
        delete[] s_data->quadVertexBufferBase;
        delete s_data;
        s_data = nullptr;
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

        s_data->quadShader->Bind();
        s_data->quadShader->SetMat4("uViewProjection", viewProj);

        s_data->quadIndexCount = 0;
        s_data->quadVertexBufferPtr = s_data->quadVertexBufferBase;

        s_data->textureSlotIndex = 1;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_data->quadShader->Bind();
        s_data->quadShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());

        s_data->quadIndexCount = 0;
        s_data->quadVertexBufferPtr = s_data->quadVertexBufferBase;

        s_data->textureSlotIndex = 1;
    }


    void Renderer2D::EndScene()
    {
        const uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(s_data->quadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_data->quadVertexBufferBase));
        s_data->quadVertexBuffer->SetData(s_data->quadVertexBufferBase, dataSize);
        Flush();
    }

    void Renderer2D::Flush()
    {
        if (s_data->quadIndexCount)
        {
            uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(s_data->quadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_data->quadVertexBufferBase));
            s_data->quadVertexBuffer->SetData(s_data->quadVertexBufferBase, dataSize);

            for (uint32_t i = 0; i < s_data->textureSlotIndex; i++)
                s_data->textureSlots[i]->Bind(i);

            s_data->quadShader->Bind();
            RenderCommand::DrawIndexed(s_data->quadVertexArray, s_data->quadIndexCount);
            s_data->stats.drawCalls++;
        }
    }

    void Renderer2D::FlushAndReset()
    {
        EndScene();

        s_data->quadIndexCount = 0;
        s_data->quadVertexBufferPtr = s_data->quadVertexBufferBase;

        s_data->textureSlotIndex = 1;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f,}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        DrawQuad(transform, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintcolor)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintcolor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
             * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        DrawQuad(transform, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        constexpr float textureIndex = 0.0f;
        constexpr float tilingFactor = 1.0f;

        if (s_data->quadIndexCount >= Renderer2DData::maxIndices)
            FlushAndReset();

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_data->quadVertexBufferPtr->position = transform * s_data->quadVertexPositions[i];
            s_data->quadVertexBufferPtr->color = color;
            s_data->quadVertexBufferPtr->texCoord = textureCoords[i];
            s_data->quadVertexBufferPtr->texIndex = textureIndex;
            s_data->quadVertexBufferPtr->tilingFactor = tilingFactor;
            s_data->quadVertexBufferPtr++;
        }

        s_data->quadIndexCount += 6;

        s_data->stats.quadCount++;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor,
        const glm::vec4& tintColor)
    {
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        float textureIndex = 0.0f;

        if (s_data->quadIndexCount >= Renderer2DData::maxIndices)
            FlushAndReset();

        for (uint32_t i = 1; i < s_data->textureSlotIndex; i++)
        {
            if (*s_data->textureSlots[i] == *texture)
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (s_data->textureSlotIndex >= Renderer2DData::maxTextureSlots)
                FlushAndReset();

            textureIndex = static_cast<float>(s_data->textureSlotIndex);
            s_data->textureSlots[s_data->textureSlotIndex] = texture;
            s_data->textureSlotIndex++;
        }

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_data->quadVertexBufferPtr->position = transform * s_data->quadVertexPositions[i];
            s_data->quadVertexBufferPtr->color = tintColor;
            s_data->quadVertexBufferPtr->texCoord = textureCoords[i];
            s_data->quadVertexBufferPtr->texIndex = textureIndex;
            s_data->quadVertexBufferPtr->tilingFactor = tilingFactor;
            s_data->quadVertexBufferPtr++;
        }

        s_data->quadIndexCount += 6;

        s_data->stats.quadCount++;
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
        const glm::vec4& color)
    {
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        DrawQuad(transform, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
        const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
        const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        DrawQuad(transform, texture, tilingFactor, tintColor);
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_data->stats;
    }

    void Renderer2D::ResetStats()
    {
        memset(&s_data->stats, 0, sizeof(Statistics));
    }
} // Apollo