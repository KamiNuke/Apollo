#include "OrthographicCameraController.h"

#include "Input/KeyCodes.h"
#include "Input/Input.h"

namespace Apollo
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_aspectRatio(aspectRatio),
            m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel),
            m_rotation(rotation)
    {

    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        if (Input::IsKeyPressed(APOLLO_KEY_A))
        {
            m_cameraPosition.x -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
            m_cameraPosition.y -= sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(APOLLO_KEY_D))
       {
            m_cameraPosition.x += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
            m_cameraPosition.y += sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
       }

        if (Input::IsKeyPressed(APOLLO_KEY_W))
        {
            m_cameraPosition.x += -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
            m_cameraPosition.y += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(APOLLO_KEY_S))
        {
            m_cameraPosition.x -= -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
            m_cameraPosition.y -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
        }

		if (m_rotation)
		{
			if (Input::IsKeyPressed(APOLLO_KEY_Q))
				m_cameraRotation += m_cameraRotationSpeed * ts;
			if (Input::IsKeyPressed(APOLLO_KEY_E))
				m_cameraRotation -= m_cameraRotationSpeed * ts;

			if (m_cameraRotation > 180.0f)
				m_cameraRotation -= 360.0f;
			else if (m_cameraRotation <= -180.0f)
				m_cameraRotation += 360.0f;

			m_camera.SetRotation(m_cameraRotation);
		}

        m_camera.SetPosition(m_cameraPosition);
        m_cameraTranslationSpeed = m_zoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_zoomLevel -= e.GetOffsetY();
        m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
        return false;
    }

    void OrthographicCameraController::OnResize(float width, float height)
    {
        m_aspectRatio = width / height;
        m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
    }
} // Apollo