#pragma once
#include "glad/glad.h"
#include "Renderer/Buffer.h"

namespace Apollo
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;
        void Bind() const override;
        void Unbind() const override;

        void SetData(const void* data, uint32_t size) override;

        [[nodiscard]] const BufferLayout& GetLayout() const override { return m_layout; };
        void SetLayout(const BufferLayout& layout) override { m_layout = layout; };

    private:
        uint32_t m_bufferID;
        BufferLayout m_layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;
        void Bind() const override;
        void Unbind() const override;

        [[nodiscard]] inline uint32_t GetCount() const override;

    private:
        uint32_t m_bufferID;
        uint32_t m_count;
    };
} // Apollo
