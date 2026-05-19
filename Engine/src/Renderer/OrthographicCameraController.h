#pragma once
#include "OrthographicCamera.h"
#include "Core/Timestep.h"
#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Event/MouseEvent.h"

namespace Apollo
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        [[nodiscard]] OrthographicCamera& GetCamera() { return m_camera; }
        [[nodiscard]] const OrthographicCamera& GetCamera() const { return m_camera; }

        float GetZoomLevel() const { return m_zoomLevel; }
        void SetZoomLevel(float zoomLevel) { m_zoomLevel = zoomLevel; }
    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
        void OnResize(float width, float height);
    private:
        float m_aspectRatio;
        float m_zoomLevel = 1.0f;
        OrthographicCamera m_camera;

        bool m_rotation;

        glm::vec3 m_cameraPosition = {0.0f, 0.0f, 0.0f};
        float m_cameraRotation = 0.0f;
        float m_cameraTranslationSpeed = 1.0f, m_cameraRotationSpeed = 5.0f;
    };
} // Apollo
