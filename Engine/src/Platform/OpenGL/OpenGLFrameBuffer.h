#pragma once
#include "Renderer/FrameBuffer.h"

namespace Apollo
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(uint32_t width, uint32_t height);
        ~OpenGLFrameBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        void Resize(uint32_t width, uint32_t height) override;
        [[nodiscard]] unsigned int GetTexture() const { return m_texture; }

    private:
        unsigned int m_frameBuffer, m_texture, m_renderBuffer;
    };
} // Apollo
