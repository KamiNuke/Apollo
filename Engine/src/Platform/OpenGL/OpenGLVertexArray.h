#pragma once
#include "Renderer/VertexArray.h"

namespace Apollo
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) override;

        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() override { return m_vertexBuffers;};
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() override { return m_indexBuffer; };

    private:
        uint32_t m_vertexArrayID;
        uint32_t m_vertexBufferIndex = 0;

        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
    };
} // Apollo
