#pragma once
#include "Renderer/Camera.h"

namespace Apollo
{
    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1};
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float size, float nearClip, float farClip);
        void SetPerspective(float verticalFOV, float nearClip, float farClip);

        void SetViewportSize(uint32_t width, uint32_t height);

        float GetOrthographicSize() const { return m_orthographicSize; }
        void SetOrthographicSize(float size) { m_orthographicSize = size; RecalculateProjection(); }
        float GetOrthographicNearClip() const { return m_orthographicNear; }
        void SetOrthographicNearClip(float nearClip) { m_orthographicNear = nearClip; RecalculateProjection(); }
        float GetOrthographicFarClip() const { return m_orthographicFar; }
        void SetOrthographicFarClip(float farClip) { m_orthographicFar = farClip; RecalculateProjection(); }

        float GetPerspectiveVerticalFOV() const { return m_perspectiveFOV; }
        void SetPerspectiveVerticalFOV(float fov) { m_perspectiveFOV = fov; RecalculateProjection(); }
        float GetPerspectiveNearClip() const { return m_perspectiveNear; }
        void SetPerspectiveNearClip(float nearClip) { m_perspectiveNear = nearClip; RecalculateProjection(); }
        float GetPerspectiveFarClip() const { return m_perspectiveFar; }
        void SetPerspectiveFarClip(float farClip) { m_perspectiveFar = farClip; RecalculateProjection(); }

        ProjectionType GetProjectionType() const { return m_projectionType; }
        void SetProjectionType(ProjectionType type) { m_projectionType = type; RecalculateProjection(); }

    private:
        void RecalculateProjection();
    private:
        ProjectionType m_projectionType = ProjectionType::Orthographic;

        float m_orthographicSize = 10.0f;
        float m_orthographicNear = -1.0f, m_orthographicFar = 1.0f;

        float m_perspectiveFOV = glm::radians(45.0f);
        float m_perspectiveNear = 0.01f, m_perspectiveFar = 1000.0f;

        float m_aspectRatio = 0.0f;
    };
} // Apollo
