#pragma once
#include "layer.h"
#include "event/mouseevent.h"

namespace Apollo
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void onAttach() override;
        void onDetach() override;
        void onImGuiRender() override;

        void begin();
        void end();
    private:
        float m_time = 0.0f;
    };
} // Apollo
