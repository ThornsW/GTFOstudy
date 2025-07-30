#pragma once
#include <map>
#include "..\libraries\imgui\imgui.h"
#include <vector>
#include <string>

namespace ESP
{
    struct AgentESPSection
    {
        std::string type;
        bool show = true;

        bool showBoxes = true;
        ImVec4 boxesColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 boxesOutlineColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

        bool showHealthBar = true;
        int healthBarThickness = 3;
        bool healthBarText = true;
        bool healthBarTextFull = true;
        ImVec4 healthBarTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 healthBarTextOutlineColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

        bool showInfo = true;
        bool showName = true;
        bool showType = true;
        bool showHealth = true;
        bool showDistance = true;
        ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 textOutlineColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

        int renderDistance = 30;

        bool showSkeleton = true;
        ImVec4 skeletonColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        int skeletonRenderDistance = 30;
        float skeletonThickness = 1.0f;

        AgentESPSection(std::string type)
        {
            this->type = type;
        }
    };

    struct AgentESP
    {
        AgentESPSection visibleSec = AgentESPSection("Visible");
        AgentESPSection nonVisibleSec = AgentESPSection("NonVisible");
    };

    extern std::map<std::string, std::string> espItemsReverse;

    extern AgentESP enemyESP;
}