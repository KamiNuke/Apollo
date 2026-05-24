#include <Apollo.h>
#include <main.h>

#include "EditorLayer.h"

namespace Apollo
{
    class Editor : public Application
    {
    public:
        Editor(const ApplicationSpecification& spec)
            : Application(spec)
        {
            PushLayer(new EditorLayer());
        }
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        ApplicationSpecification spec;
        spec.name = "Apollo Editor";
        spec.cmdArgs = args;

        return new Editor(spec);
    }
}
