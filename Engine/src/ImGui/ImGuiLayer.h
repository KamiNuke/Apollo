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

        void BlockEvents(bool block) { m_blockEvents = block; }

        void SetDarkThemeColors();
        void SetLightThemeColors();
        void SetStyle();
    private:
        bool m_blockEvents = true;
    };
} // Apollo
