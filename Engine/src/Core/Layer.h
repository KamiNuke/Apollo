#pragma once
#include <string>

#include "Timestep.h"
#include "Event/Event.h"

namespace Apollo
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}
        [[nodiscard]] const inline std::string& GetName() const { return m_name; }
    protected:
        std::string m_name;
    };
} // Apollo
