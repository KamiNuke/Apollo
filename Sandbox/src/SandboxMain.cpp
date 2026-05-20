#include "SandboxMain.h"
#include "main.h"

#include "imgui.h"
#include "Sandbox2D.h"
#include "Renderer/OrthographicCameraController.h"


class TestLayer : public Apollo::Layer
{
public:
    TestLayer()
        : Layer("Test"), m_cameraController(1280.0f / 720.0f, true),
            m_squarePosition(1.0f), m_squareColor(1.0f)
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

        m_vertexArray = Apollo::VertexArray::Create();
        m_vertexArray->Bind();

        Apollo::Ref<Apollo::VertexBuffer> m_vertexBuffer = Apollo::VertexBuffer::Create(vertices, sizeof(vertices));
        Apollo::Ref<Apollo::IndexBuffer> m_indexBuffer = Apollo::IndexBuffer::Create(
            indices, sizeof(indices) / sizeof(uint32_t));

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


        float vertices2[5*4]
        {
            -0.85f, -0.85f, 0.0f, 0.0f, 0.0f,
             0.85f, -0.85f, 0.0f, 1.0f, 0.0f,
             0.85f,  0.85f, 0.0f, 1.0f, 1.0f,
            -0.85f,  0.85f, 0.0f, 0.0f, 1.0f,
         };

        unsigned int indices2[6]
        {
            0, 1, 2,
            2, 3, 0,
        };

        m_squareVA = Apollo::VertexArray::Create();
        m_squareVA->Bind();

        Apollo::Ref<Apollo::VertexBuffer> m_squareVB = Apollo::VertexBuffer::Create(vertices2, sizeof(vertices2));
        Apollo::Ref<Apollo::IndexBuffer> m_squareVI = Apollo::IndexBuffer::Create(
            indices2, sizeof(indices2) / sizeof(uint32_t));

        {
            Apollo::BufferLayout layout =
            {
                { Apollo::ShaderDataType::Float3, "aPos"},
                { Apollo::ShaderDataType::Float2, "aTexCoord"}
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

        m_shader = Apollo::Shader::Create("test", vertexSrc, fragSrc);

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

            uniform vec3 uColor;

            void main()
            {
                FragColor = vec4(uColor, 1.0f);
            }
        )";

        m_shader2 = Apollo::Shader::Create("test", vertexSrc2, fragSrc2);
        m_fbo = Apollo::CreateRef<Apollo::OpenGLFrameBuffer>(1280, 720);

        auto textureShader = m_shaderLibrary.Load("../../../Sandbox/shaders/Texture.glsl");
        m_texture = Apollo::Texture2D::Create("../../../Sandbox/assets/klauncher.png");

        textureShader->Bind();
        textureShader->SetInt("uTexture", 0);
    }

    void OnUpdate(Apollo::Timestep timestep) override
    {
        m_cameraController.OnUpdate(timestep);

        Apollo::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
        Apollo::RenderCommand::Clear();
        m_fbo->Bind();
        Apollo::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f ,1.0f});
        Apollo::RenderCommand::Clear();

        Apollo::Renderer::BeginScene(m_cameraController.GetCamera());

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_squareScale));
        m_shader2->Bind();
        m_shader2->SetFloat3("uColor", m_squareColor);

        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                glm::vec3 pos(i * 0.11, j * 0.11, 0.0f);
                pos = pos + m_squarePosition;
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

                Apollo::Renderer::Submit(m_shader2, m_squareVA, transform);
            }
        }

        auto textureShader = m_shaderLibrary.Get("Texture");
        m_texture->Bind();
        Apollo::Renderer::Submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(0.75f)));

        //Apollo::Renderer::Submit(m_shader, m_vertexArray);
        Apollo::Renderer::EndScene();

        m_fbo->Unbind();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Viewport");

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
            m_fbo->GetTexture(),
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x + window_width, pos.y + window_height),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
        ImGui::End();
    }

    void OnEvent(Apollo::Event& event) override
    {
        m_cameraController.OnEvent(event);
    }

private:
    Apollo::Ref<Apollo::OpenGLFrameBuffer> m_fbo;

    Apollo::ShaderLibrary m_shaderLibrary;
    Apollo::Ref<Apollo::Shader> m_shader;
    Apollo::Ref<Apollo::VertexArray> m_vertexArray;

    Apollo::Ref<Apollo::VertexArray> m_squareVA;
    Apollo::Ref<Apollo::Shader> m_shader2;

    Apollo::Ref<Apollo::Texture> m_texture;

    Apollo::OrthographicCameraController m_cameraController;

    glm::vec3 m_squarePosition;
    float m_squareScale = 0.05f;
    glm::vec3 m_squareColor;
};

SandboxMain::SandboxMain(const Apollo::Window::Properties& props)
    : Application(props)
{
    //PushLayer(new TestLayer());
    PushLayer(new Sandbox2D());
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