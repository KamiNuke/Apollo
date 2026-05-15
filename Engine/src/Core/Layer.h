#pragma once
#include <string>

#include "Event/Event.h"

namespace Apollo
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate() {}
        virtual void onImGuiRender() {}
        virtual void onEvent(Event& event) {}
        [[nodiscard]] const inline std::string& getName() const { return m_name; }
    protected:
        std::string m_name;
    };
} // Apollo
