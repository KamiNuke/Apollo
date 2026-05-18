#include "SandboxMain.h"

#include "defines.h"
#include "imgui.h"
#include "Event/KeyEvent.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

class TestLayer : public Apollo::Layer
{
public:
    TestLayer()
        : Layer("Test"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f)
    {

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

        m_vertexArray.reset(Apollo::VertexArray::Create());
        m_vertexArray->Bind();

        std::shared_ptr<Apollo::VertexBuffer> m_vertexBuffer;
        std::shared_ptr<Apollo::IndexBuffer> m_indexBuffer;
        m_vertexBuffer.reset(Apollo::VertexBuffer::Create(vertices, sizeof(vertices)));
        m_indexBuffer.reset(Apollo::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        {
            Apollo::BufferLayout layout =
            {
                { Apollo::ShaderDataType::Float3, "aPos"},
                { Apollo::ShaderDataType::Float3, "aColor"},
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

        m_squareVA.reset(Apollo::VertexArray::Create());
        m_squareVA->Bind();

        std::shared_ptr<Apollo::VertexBuffer> m_squareVB;
        std::shared_ptr<Apollo::IndexBuffer> m_squareVI;
        m_squareVB.reset(Apollo::VertexBuffer::Create(vertices2, sizeof(vertices2)));
        m_squareVI.reset(Apollo::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));

        {
            Apollo::BufferLayout layout =
            {
                { Apollo::ShaderDataType::Float3, "aPos"},
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

            uniform mat4 uViewProjection;

            out vec3 vColor;

            void main()
            {
                gl_Position = uViewProjection * vec4(aPos.x + 0.5, aPos.y, aPos.z, 1.0);
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

        m_shader = std::make_shared<Apollo::Shader>(vertexSrc, fragSrc);

        std::string vertexSrc2 = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;

            uniform mat4 uViewProjection;

            out vec3 ourColor;

            void main()
            {
                gl_Position = uViewProjection * vec4(aPos, 1.0f);
                ourColor = aPos;
            }
        )";

        std::string fragSrc2 = R"(
            #version 330 core
            in vec3 ourColor;
            out vec4 FragColor;
            void main()
            {
                FragColor = vec4(ourColor, 1.0f);
            }
        )";

        m_shader2 = std::make_shared<Apollo::Shader>(vertexSrc2, fragSrc2);
    }

    void OnUpdate(Apollo::Timestep timestep) override
    {
        LOGGER_INFO("Delta time: {0} ({1})", timestep.GetSeconds(), timestep.GetMilliseconds());

        if (Apollo::Input::IsKeyPressed(APOLLO_KEY_LEFT))
            m_cameraPosition.x += m_cameraMoveSpeed * timestep;
        else if (Apollo::Input::IsKeyPressed(APOLLO_KEY_RIGHT))
            m_cameraPosition.x -= m_cameraMoveSpeed * timestep;

        if (Apollo::Input::IsKeyPressed(APOLLO_KEY_UP))
            m_cameraPosition.y -= m_cameraMoveSpeed * timestep;
        else if (Apollo::Input::IsKeyPressed(APOLLO_KEY_DOWN))
            m_cameraPosition.y += m_cameraMoveSpeed * timestep;

        if (Apollo::Input::IsKeyPressed(APOLLO_KEY_A))
           m_rotation -= m_cameraRotationSpeed * timestep;
        else if (Apollo::Input::IsKeyPressed(APOLLO_KEY_D))
            m_rotation += m_cameraRotationSpeed * timestep;


        Apollo::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
        Apollo::RenderCommand::Clear();

        m_camera.SetPosition(m_cameraPosition);
        m_camera.SetRotation(m_rotation);

        Apollo::Renderer::BeginScene(m_camera);

        Apollo::Renderer::Submit(m_shader2, m_squareVA, glm::mat4(1.0f));
        Apollo::Renderer::Submit(m_shader, m_vertexArray, glm::mat4(1.0f));

        Apollo::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {
    }

    void OnEvent(Apollo::Event& event) override
    {
    }

private:
    std::shared_ptr<Apollo::Shader> m_shader;
    std::shared_ptr<Apollo::VertexArray> m_vertexArray;

    std::shared_ptr<Apollo::VertexArray> m_squareVA;
    std::shared_ptr<Apollo::Shader> m_shader2;

    Apollo::OrthographicCamera m_camera;
    glm::vec3 m_cameraPosition;
    float m_cameraMoveSpeed = 5.0f;

    float m_rotation = 0.0f;
    float m_cameraRotationSpeed = 180.0f;
};

SandboxMain::SandboxMain(const Apollo::Window::Properties& props)
    : Application(props)
{
    PushLayer(new TestLayer());
}

SandboxMain::~SandboxMain()
{
}

void SandboxMain::OnUpdate()
{
}

void SandboxMain::OnRender()
{
}

namespace Apollo
{
    Application* CreateApplication()
    {
        Apollo::Window::Properties props;
        props.title = "Sandbox";

        return new SandboxMain(props);
    }
}