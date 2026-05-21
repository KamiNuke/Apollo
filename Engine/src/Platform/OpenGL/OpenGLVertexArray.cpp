#include "glad/glad.h"

#include "OpenGLVertexArray.h"

namespace Apollo
{
    static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType& type)
    {
        switch (type)
        {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool:  return GL_BOOL;
            default: break;
        }

        assert("Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_vertexArrayID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_vertexArrayID);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_vertexArrayID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer)
    {
        assert(!buffer->GetLayout().GetElements().empty() && "Vertex buffer has no layout!");

        glBindVertexArray(m_vertexArrayID);
        buffer->Bind();

        const auto& layout = buffer->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.type)
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                {
                    glEnableVertexAttribArray(m_vertexBufferIndex);
                    glVertexAttribPointer(m_vertexBufferIndex,
                    element.GetElementCount(),
                        ShaderDataTypeToOpenGLBaseType(element.type),
                        element.normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        reinterpret_cast<const void*>(element.offset)
                    );
                    m_vertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(m_vertexBufferIndex);
                    glVertexAttribIPointer(m_vertexBufferIndex,
                    element.GetElementCount(),
                        ShaderDataTypeToOpenGLBaseType(element.type),
                        layout.GetStride(),
                        reinterpret_cast<const void*>(element.offset)
                    );
                    m_vertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    uint8_t count = element.GetElementCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(m_vertexBufferIndex);
                        glVertexAttribPointer(m_vertexBufferIndex,
                        count,
                            ShaderDataTypeToOpenGLBaseType(element.type),
                            element.normalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(),
                            reinterpret_cast<void*>(element.offset + sizeof(float) * count * i)
                        );
                        glVertexAttribDivisor(m_vertexBufferIndex, 1);
                        m_vertexBufferIndex++;
                    }
                    break;
                }
                default:
                    assert("Unknown ShaderDataType!");
            }
        }

        m_vertexBuffers.push_back(buffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer)
    {
        glBindVertexArray(m_vertexArrayID);
        buffer->Bind();

        m_indexBuffer = buffer;
    }
} // Apollo