#include "ResourceManager.h"

namespace Apollo
{
    ResourceManager* ResourceManager::s_instance = nullptr;

    ResourceManager& ResourceManager::Get()
    {
        if (!s_instance)
            s_instance = new ResourceManager{};

        return *s_instance;
    }

    Ref<Texture2D> ResourceManager::GetTexture2D(const std::string& path)
    {
        for (const auto& [key, value] : m_map)
        {
            if (key == path)
                return m_map[path];
        }

        return nullptr;
    }
} // Apollo