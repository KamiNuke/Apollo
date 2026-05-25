#pragma once
#include <unordered_map>
#include <string>

#include "Base.h"
#include "Renderer/Texture.h"

namespace Apollo
{
    class ResourceManager
    {
    public:
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager operator=(ResourceManager&) = delete;

        static ResourceManager& Get();

        void AddTexture2D(const std::string& path, const Ref<Texture2D>& texture) { m_map[path] = texture; }
        Ref<Texture2D> GetTexture2D(const std::string& path);
    private:
        ResourceManager() {}

        static ResourceManager* s_instance;
        std::unordered_map<std::string, Ref<Texture2D>> m_map;
    };
} // Apollo
