#pragma once
#include <filesystem>

namespace Apollo
{
    class ContentBrowserPanel
    {
    public:
        ContentBrowserPanel();

        void OnImGuiRender();
    private:
        std::filesystem::path m_currentDirectory;
    };
} // Apollo
