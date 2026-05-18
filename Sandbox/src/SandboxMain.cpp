#include "SandboxMain.h"

#include "defines.h"
#include "imgui.h"
#include "Event/KeyEvent.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

class TestLayer : public Apollo::Layer
{
public:
    TestLayer()
        : Layer("Test"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_squarePosition(1.0f), m_cameraPosition(0.0f)
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
            uniform mat4 uTransform;

            out vec3 vColor;

            void main()
            {
                gl_Position = uViewProjection * uTransform * vec4(aPos.x + 0.5, aPos.y, aPos.z, 1.0);
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
            uniform mat4 uTransform;

            out vec3 ourColor;

            void main()
            {
                gl_Position = uViewProjection * uTransform * vec4(aPos, 1.0f);
                ourColor = aPos;
            }
        )";

        std::string fragSrc2 = R"(
            #version 330 core
            in vec3 ourColor;
            out vec4 FragColor;

            uniform vec4 gColor;

            void main()
            {
                FragColor = gColor;
            }
        )";

        m_shader2 = std::make_shared<Apollo::Shader>(vertexSrc2, fragSrc2);

        m_fbo = std::make_shared<Apollo::OpenGLFrameBuffer>(1280, 720);
    }

    void OnUpdate(Apollo::Timestep timestep) override
    {
        if (Apollo::Input::IsKeyPressed(APOLLO_KEY_LEFT))
            m_cameraPosition.x += m_cameraMoveSpeed * timestep;
        else if (Apollo::Input::IsKeyPressed(APOLLO_KEY_RIGHT))
            m_cameraPosition.x -= m_cameraMoveSpeed * timestep;

        if (Apollo::Input::IsKeyPressed(APOLLO_KEY_UP))
            m_cameraPosition.y -= m_cameraMoveSpeed * timestep;
        else if (Apollo::Input::IsKeyPressed(APOLLO_KEY_DOWN))
            m_cameraPosition.y += m_cameraMoveSpeed * timestep;

        if (Apollo::Input::IsKeyPressed(APOLLO_KEY_I))
           m_rotation -= m_cameraRotationSpeed * timestep;
        else if (Apollo::Input::IsKeyPressed(APOLLO_KEY_J))
            m_rotation += m_cameraRotationSpeed * timestep;

        if (Apollo::Input::IsKeyPressed(APOLLO_KEY_A))
            m_squarePosition.x -= 1.1f * timestep;
        else if (Apollo::Input::IsKeyPressed(APOLLO_KEY_D))
            m_squarePosition.x += 1.1f * timestep;

        if (Apollo::Input::IsKeyPressed(APOLLO_KEY_W))
            m_squarePosition.y += 1.1f * timestep;
        else if (Apollo::Input::IsKeyPressed(APOLLO_KEY_S))
            m_squarePosition.y -= 1.1f * timestep;

        if (Apollo::Input::IsKeyPressed(APOLLO_KEY_R))
            m_squareScale += 0.5f * timestep;
        else if (Apollo::Input::IsKeyPressed(APOLLO_KEY_T))
            m_squareScale -= 0.5f * timestep;

        Apollo::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
        Apollo::RenderCommand::Clear();
        m_fbo->Bind();
        Apollo::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
        Apollo::RenderCommand::Clear();

        m_camera.SetPosition(m_cameraPosition);
        m_camera.SetRotation(m_rotation);

        Apollo::Renderer::BeginScene(m_camera);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_squareScale));

        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                glm::vec3 pos(i * 0.11, j * 0.11, 0.0f);
                pos = pos + m_squarePosition;
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                if (i % 2 == 0)
                    m_shader2->SetFloat4("gColor", {0.8f, 0.2f, 0.3f, 1.0f});
                else
                    m_shader2->SetFloat4("gColor", {0.1f, 0.3f, 0.98f, 1.0f});

                Apollo::Renderer::Submit(m_shader2, m_squareVA, transform);
            }
        }

        Apollo::Renderer::Submit(m_shader, m_vertexArray);
        Apollo::Renderer::EndScene();

        m_fbo->Unbind();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("My Scene");

        // we access the ImGui window size
        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;

        // we rescale the framebuffer to the actual window size here and reset the glViewport
        m_fbo->Resize(window_width, window_height);
        Apollo::RenderCommand::SetViewport(0, 0, window_width, window_height);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        // and here we can add our created texture as image to ImGui
        // unfortunately we need to use the cast to void* or I didn't find another way tbh
        ImGui::GetWindowDrawList()->AddImage(
            (void *)m_fbo->GetTexture(),
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x + window_width, pos.y + window_height),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::End();
    }

    void OnEvent(Apollo::Event& event) override
    {
    }

private:
    std::shared_ptr<Apollo::OpenGLFrameBuffer> m_fbo;

    std::shared_ptr<Apollo::Shader> m_shader;
    std::shared_ptr<Apollo::VertexArray> m_vertexArray;

    std::shared_ptr<Apollo::VertexArray> m_squareVA;
    std::shared_ptr<Apollo::Shader> m_shader2;

    Apollo::OrthographicCamera m_camera;
    glm::vec3 m_cameraPosition;
    float m_cameraMoveSpeed = 5.0f;

    float m_rotation = 0.0f;
    float m_cameraRotationSpeed = 5.0f;

    glm::vec3 m_squarePosition;
    float m_squareScale = 0.05f;
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