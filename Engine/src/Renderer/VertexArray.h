#pragma once
#include <memory>

#include "Buffer.h"

namespace Apollo
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() = 0;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() = 0;

        static Scope<VertexArray> Create();
    };
} // Apollo
