#pragma once
#include <Apollo.h>

class Sandbox2D : public Apollo::Layer
{
public:
    Sandbox2D();
    ~Sandbox2D() override = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Apollo::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Apollo::Event& event) override;

private:
    Apollo::OrthographicCameraController m_cameraController;

    Apollo::Ref<Apollo::Shader> m_flatColorShader;
    Apollo::Ref<Apollo::VertexArray> m_squareVA;
    Apollo::Ref<Apollo::Texture2D> m_texture;
    glm::vec3 m_texturePosition {0.0f, 0.0f, 0.0f};

    glm::vec4 m_squareColor {0.9f, 0.2f, 0.4f, 1.0f};
};
