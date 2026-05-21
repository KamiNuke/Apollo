#pragma once
#include "../Core/Layer.h"
#include "Event/MouseEvent.h"

namespace Apollo
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;

        void OnEvent(Event& event) override;

        void Begin();
        void End();

    private:
        float m_time = 0.0f;
    };
} // Apollo
