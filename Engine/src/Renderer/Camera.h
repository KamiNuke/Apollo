#pragma once
#include "glm/glm.hpp"

namespace Apollo
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
            : m_projectionMatrix(projection) {}

        virtual ~Camera() = default;

        [[nodiscard]] glm::mat4 GetProjection() const { return m_projectionMatrix; }
    protected:
        glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
    };
} // Apollo
