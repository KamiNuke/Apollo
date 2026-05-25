#include "Font.h"




namespace Apollo
{
    void Font::DrawText(const std::string& text, ImVec2 pos)
    {
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(textSize, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGui::Begin("###text", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);

        ImGui::Text(text.c_str());

        ImGui::End();

        ImGui::PopStyleVar(2);
    }
} // Apollo