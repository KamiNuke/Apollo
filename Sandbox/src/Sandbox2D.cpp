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

    Apollo::Renderer2D::BeginScene(m_cameraController.GetCamera());

    //Apollo::Renderer2D::DrawQuad({0.0f, 1.0f}, {1.0f, 0.30f}, m_squareColor);
    //Apollo::Renderer2D::DrawRotatedQuad({-1.0f, 0.0f}, {0.50f, 1.0f},glm::radians(-60.0f),  m_squareColor);
    //Apollo::Renderer2D::DrawRotatedQuad({m_texturePosition.x - 0.5f, m_texturePosition.y, -0.1}, {1.0f, 1.0f}, glm::radians(45.0f), m_texture);

    Apollo::Renderer2D::DrawQuad(m_texturePosition, {1.0f, 1.0f}, m_texture, 0.5f, m_squareColor);
    Apollo::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.50f, 1.0f}, m_squareColor);
    Apollo::Renderer2D::DrawQuad({0.0f, 1.0f}, {1.0f, 0.30f}, m_squareColor);


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
