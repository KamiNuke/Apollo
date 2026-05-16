#include "Application.h"

#include <memory>

#include "Logger/Log.h"
#include "glad/glad.h"
#include "defines.h"
#include "Input.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

namespace Apollo
{
    Application* Application::s_instance = nullptr;

    Application::Application(const Window::Properties& props)
    {
        assert(!s_instance && "Application already exists");
        s_instance = this;

        m_window = std::unique_ptr<Window>(Window::Create());
        m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_imGuiLayer = new ImGuiLayer();
        PushOverlay(m_imGuiLayer);

        float vertices[6*3]
        {
            -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.0f,   0.5f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
        };

        unsigned int indices[3]
        {
            0, 1, 2,
        };

        m_vertexArray.reset(VertexArray::Create());
        m_vertexArray->Bind();

        std::shared_ptr<VertexBuffer> m_vertexBuffer;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        m_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        m_indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        {
            BufferLayout layout =
            {
                { ShaderDataType::Float3, "aPos"},
                { ShaderDataType::Float3, "aColor"},
            };
            m_vertexBuffer->SetLayout(layout);
        }

        m_vertexArray->AddVertexBuffer(m_vertexBuffer);
        m_vertexArray->SetIndexBuffer(m_indexBuffer);
        m_vertexArray->Unbind();


        float vertices2[4*3]
        {
            -0.85f, -0.85f, 0.0f,
             0.85f, -0.85f, 0.0f,
             0.85f,  0.85f, 0.0f,
            -0.85,   0.85f, 0.0f
        };

        unsigned int indices2[6]
        {
            0, 1, 2,
            2, 3, 0,
        };

        m_squareVA.reset(VertexArray::Create());
        m_squareVA->Bind();

        std::shared_ptr<VertexBuffer> m_squareVB;
        std::shared_ptr<IndexBuffer> m_squareVI;
        m_squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
        m_squareVI.reset(IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));

        {
            BufferLayout layout =
            {
                { ShaderDataType::Float3, "aPos"},
            };
            m_squareVB->SetLayout(layout);
        }

        m_squareVA->AddVertexBuffer(m_squareVB);
        m_squareVA->SetIndexBuffer(m_squareVI);
        m_squareVA->Unbind();

        std::string vertexSrc = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;

            out vec3 vColor;

            void main()
            {
                gl_Position = vec4(aPos, 1.0);
                vColor = aColor;
            }
        )";

        std::string fragSrc = R"(
            #version 330 core
            in vec3 vColor;
            out vec4 FragColor;
            void main()
            {
                FragColor = vec4(vColor, 1.0);
            }
        )";

        m_shader = std::make_shared<Shader>(vertexSrc, fragSrc);

        std::string vertexSrc2 = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;

            out vec3 vColor;

            void main()
            {
                gl_Position = vec4(aPos, 1.0);
                vColor = aPos;
            }
        )";

        std::string fragSrc2 = R"(
            #version 330 core
            in vec3 vColor;
            out vec4 FragColor;
            void main()
            {
                FragColor = vec4(vColor, 1.0);
            }
        )";

        m_shader2 = std::make_shared<Shader>(vertexSrc2, fragSrc2);
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_isRunning)
        {
            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
            RenderCommand::Clear();

            Renderer::BeginScene();

            Renderer::Submit(m_shader2, m_squareVA);
            Renderer::Submit(m_shader, m_vertexArray);

            Renderer::EndScene();

            for (Layer* layer : m_layerStack)
                layer->OnUpdate();

            m_imGuiLayer->Begin();
            for (Layer* layer : m_layerStack)
                layer->OnImGuiRender();
            m_imGuiLayer->End();

            m_window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        //APOLLO_LOGGER_TRACE("{0}", e);

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
        {
            if (e.handled)
                break;
            (*it)->OnEvent(e);
        }

    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_layerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_isRunning = false;
        return true;
    }
} // ApolloEvent& event