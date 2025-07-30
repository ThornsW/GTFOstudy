#include "pch-il2cpp.h"
#include "pipeline/gui/tabs/SettingsTAB.h"
#include <imgui/imgui.h>
#include "pipeline/gui/GUITheme.h" 
#include "pipeline/settings.h"
#include "main.h"
#include <iostream>

void SettingsTAB::Render()
{
    if (ImGui::BeginTabItem("Settings")) {

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Version: 0.3.0");
        ImGui::Spacing();
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 550.0f);
        ImGui::Text("Project creator:ThornsW");
        ImGui::PopTextWrapPos();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Special thanks: djkaty provided the Il2CppInspector tool");
        ImGui::Spacing();
        ImGui::Text("Special thanks: Jadis0x provided the Il2CppInspectorPro tool");
        ImGui::Spacing();
        ImGui::Text("This project is a copy of the GTFOHax project by mankool0");
        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Checkbox("Show Unity Logs", &settings.bEnableUnityLogs); // Test checkbox
        ImGui::Spacing();

        if (ImGui::Button("Unhook"))
        {
            SetEvent(hUnloadEvent);
        }

        if (ImGui::Button("Blog"))
        {
            app::Application_OpenURL(reinterpret_cast<app::String*>(il2cpp_string_new("https://thornsw.online")), nullptr);
        }

        ImGui::EndTabItem();
    }
}
