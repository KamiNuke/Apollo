#pragma once
#include <memory>

#include "OrthographicCamera.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Apollo
{
    class Renderer
    {
    public:
        static void Init();

        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void OnWindowResize(uint32_t width, uint32_t height);

        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        [[nodiscard]] static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 viewProjectionMatrix;
        };

        static SceneData* s_sceneData;
    };
} // Apollo
