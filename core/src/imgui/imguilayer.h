#pragma once
#include "layer.h"
#include "event/mouseevent.h"

namespace Apollo
{
    class ImguiLayer : public Layer
    {
    public:
        ImguiLayer();
        ~ImguiLayer() override;

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;
        void onEvent(Event& event) override;
    private:
        float m_time = 0.0f;
    };
} // Apollo
