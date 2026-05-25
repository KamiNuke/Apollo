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
        auto it = m_map.find(path);
        if (it != m_map.end())
        {
            if (auto texture = it->second.lock())
                return texture;

            m_map.erase(it);
        }

        return nullptr;
    }

} // Apollo