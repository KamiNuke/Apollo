#include <Apollo.h>
#include <main.h>

#include "EditorLayer.h"

namespace Apollo
{
    class Editor : public Application
    {
    public:
        Editor(const Window::Properties& props)
            : Application(props)
        {
            PushLayer(new EditorLayer());
        }
    };

    Application* CreateApplication()
    {
        Window::Properties props;
        props.title = "Apollo Editor";

        return new Editor(props);
    }
}
