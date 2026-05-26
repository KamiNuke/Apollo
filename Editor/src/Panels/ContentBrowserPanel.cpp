#include "ContentBrowserPanel.h"

#include <string>
#include <filesystem>
#include "imgui.h"
#include "Logger/Log.h"


namespace Apollo
{
    static std::filesystem::path s_assetPath { "." };

    ContentBrowserPanel::ContentBrowserPanel()
        : m_currentDirectory(s_assetPath) {}

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Content Browser");

        if (m_currentDirectory != std::filesystem::path(s_assetPath))
        {
            if (ImGui::Button("<-"))
            {
                m_currentDirectory = m_currentDirectory.parent_path();
            }
        }

        for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
        {
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, s_assetPath);
            std::string filenameString = relativePath.filename().string();

            bool clicked = ImGui::Selectable(filenameString.c_str(), false,
                                 ImGuiSelectableFlags_AllowDoubleClick);


            ImGui::PushID(filenameString.c_str());
            if (ImGui::BeginDragDropSource())
            {
                const std::filesystem::path::value_type* itemPath = relativePath.c_str();
#if defined(PLATFORM_WINDOWS)
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath,
                    (wcslen(itemPath) + 1) * sizeof(std::filesystem::path::value_type));
#else
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath,
                (strlen(itemPath) + 1) * sizeof(std::filesystem::path::value_type));
#endif
                ImGui::Text("%s", filenameString.c_str());
                ImGui::EndDragDropSource();
            }
            ImGui::PopID();

            if (clicked && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if (directoryEntry.is_directory())
                    m_currentDirectory /= path.filename();
            }
        }

        ImGui::End();
    }
} // Apollo