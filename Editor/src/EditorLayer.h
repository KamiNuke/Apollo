#pragma once
#include <Apollo.h>

class EditorLayer : public Apollo::Layer
{
public:
    EditorLayer();
    ~EditorLayer() override;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Apollo::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Apollo::Event& event) override;

private:
    Apollo::OrthographicCameraController m_cameraController;

    Apollo::Ref<Apollo::FrameBuffer> m_frameBuffer;
};
