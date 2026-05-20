#include "Sandbox2D.h"

#include "imgui.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
    m_texture = Apollo::Texture2D::Create("../../../Sandbox/assets/klauncher.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Apollo::Timestep ts)
{
    m_cameraController.OnUpdate(ts);

    Apollo::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
    Apollo::RenderCommand::Clear();

    static float rotation = 0.0f;
    rotation += ts * 20.0f;

    Apollo::Renderer2D::BeginScene(m_cameraController.GetCamera());

    Apollo::Renderer2D::DrawRotatedQuad({m_texturePosition.x, m_texturePosition.y + 0.5f, -0.1}, {1.0f, 1.0f}, rotation, m_texture);
    Apollo::Renderer2D::DrawQuad({m_texturePosition.x - 0.5f, m_texturePosition.y, -0.1}, {1.0f, 1.0f}, m_texture, 1.0f, m_squareColor);
    Apollo::Renderer2D::DrawQuad({0.0f, 1.0f}, {1.0f, 0.30f}, m_squareColor);
    Apollo::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    Apollo::Renderer2D::DrawRotatedQuad({-1.0f, 0.0f}, {0.50f, 1.0f},rotation,  m_squareColor);

    Apollo::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));

    ImGui::End();
}

void Sandbox2D::OnEvent(Apollo::Event& event)
{
    m_cameraController.OnEvent(event);

}
