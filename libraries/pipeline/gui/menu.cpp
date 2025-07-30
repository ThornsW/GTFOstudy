#include "pch-il2cpp.h"
#include "menu.h"
#include "imgui/imgui.h"

#include "pipeline/settings.h"
#include "pipeline/gui/tabs/SettingsTAB.h"



namespace Menu {
	bool init = false;
	bool firstRender = true;

	void Init() {
		ImGui::SetNextWindowBgAlpha(0.7f);
		ImGui::SetNextWindowSize(ImVec2(800, 600));
		init = true;
	}

	void Render() {
		if (!init)
			Menu::Init();

		ImGui::Begin("UI design", &settings.bShowMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGui::BeginTabBar("TestGame#TopBar", ImGuiTabBarFlags_NoTabListScrollingButtons);

		if (firstRender) {
			firstRender = false;
		}
		else {
			SettingsTAB::Render();
		}

		ImGui::EndTabBar();
		ImGui::End();
	}

	void RenderESP()
	{
		if (app::GameStateManager_get_CurrentStateName(NULL) != app::eGameStateName__Enum::InLevel)
			return;

		G::localPlayer = app::PlayerManager_2_GetLocalPlayerAgent(nullptr);
		if (!G::localPlayer)
			return;

		if (G::mainCamera == NULL)
		{
			G::mainCamera = app::Camera_get_main(NULL);
			return;
		}

		G::screenHeight = app::Screen_get_height(NULL);
		G::screenWidth = app::Screen_get_width(NULL);

        G::enemyVecMtx.lock();
        std::vector<std::shared_ptr<Enemy::EnemyInfo>> enemies = Enemy::enemies;
        G::enemyVecMtx.unlock();
        std::sort(enemies.begin(), enemies.end(), [](const std::shared_ptr<Enemy::EnemyInfo> lhs, const std::shared_ptr<Enemy::EnemyInfo> rhs) {
            return lhs->distance > rhs->distance;
            });
        for (auto it = enemies.begin(); it != enemies.end(); ++it)
        {
            Enemy::EnemyInfo* enemyInfo = (*it).get();

            Enemy::Bone defBone;
            if (enemyInfo->useFallback)
                defBone = enemyInfo->fallbackBone;
            else
                defBone = enemyInfo->skeletonBones[app::HumanBodyBones__Enum::Head];

            if (!G::mainCamera || !(enemyInfo->enemyAgent->fields.m_alive) || (defBone.position.x == 0.0f && defBone.position.y == 0.0f && defBone.position.z == 0.0f))
                continue;

            if (enemyInfo->visible && ESP::enemyESP.visibleSec.show)
                RenderEnemyAgent(enemyInfo, &ESP::enemyESP.visibleSec);
            else if (!enemyInfo->visible && ESP::enemyESP.nonVisibleSec.show)
                RenderEnemyAgent(enemyInfo, &ESP::enemyESP.nonVisibleSec);
        }
	}

    void RenderEnemyAgent(Enemy::EnemyInfo* enemyInfo, ESP::AgentESPSection* espSettings)
    {
        if (enemyInfo->distance > espSettings->renderDistance)
            return;

        Enemy::Bone headBone = enemyInfo->useFallback ? enemyInfo->fallbackBone : enemyInfo->skeletonBones[app::HumanBodyBones__Enum::Head];
        ImVec2 w2sHead;
        if (!Math::WorldToScreen(headBone.position, w2sHead))
            return;

        ImVec2 w2sLeftFoot, w2sRightFoot;
        if (!enemyInfo->useFallback)
        {
            Enemy::Bone leftFootBone = enemyInfo->skeletonBones[app::HumanBodyBones__Enum::LeftFoot];
            Enemy::Bone rightFootBone = enemyInfo->skeletonBones[app::HumanBodyBones__Enum::RightFoot];
            if (!Math::WorldToScreen(leftFootBone.position, w2sLeftFoot) ||
                !Math::WorldToScreen(rightFootBone.position, w2sRightFoot))
                return;
        }
        else
        {
            w2sLeftFoot = w2sRightFoot = w2sHead;
        }

        ImVec2 min, max;
        if (!enemyInfo->useFallback)
        {
            min.y = min.x = (std::numeric_limits<float>::max)();
            max.y = max.x = -(std::numeric_limits<float>::max)();
        }
        else
        {
            min.x = max.x = w2sHead.x;
            min.y = max.y = w2sHead.y;
        }
        bool valid = true;
        for (auto const& [key, val] : enemyInfo->skeletonBones)
        {
            ImVec2 curPos;
            auto bonePos = val.position;
            if (!Math::WorldToScreen((bonePos), curPos))
            {
                valid = false;
                continue;
            }
            min.x = (std::min)(min.x, curPos.x);
            min.y = (std::min)(min.y, curPos.y);
            max.x = (std::max)(max.x, curPos.x);
            max.y = (std::max)(max.y, curPos.y);
        }

        if (espSettings->showBoxes && !enemyInfo->useFallback && valid)
        {
            ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ min.x - 1.0f, min.y - 1.0f }, ImVec2{ max.x + 1.0f, max.y + 1.0f }, ImGui::GetColorU32(espSettings->boxesOutlineColor));
            ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ min.x + 1.0f, min.y + 1.0f }, ImVec2{ max.x - 1.0f, max.y - 1.0f }, ImGui::GetColorU32(espSettings->boxesOutlineColor));
            ImGui::GetBackgroundDrawList()->AddRect(min, max, ImGui::GetColorU32(espSettings->boxesColor));
        }


        auto enemyDamage = reinterpret_cast<app::Dam_SyncedDamageBase*>(enemyInfo->enemyAgent->fields.Damage);
        std::string enemyHealth = std::to_string(llround(enemyDamage->fields._Health_k__BackingField));
        if (espSettings->showHealthBar && !enemyInfo->useFallback && valid)
        {
            float health = enemyDamage->fields._Health_k__BackingField;
            float healthMax = enemyDamage->fields._HealthMax_k__BackingField;
            float healthRatio = health / healthMax;

            float maxHeight = max.y - min.y;
            float barWidth = espSettings->healthBarThickness + 4.0f;
            ImVec2 barTopRight = { min.x - 2, min.y };
            ImVec2 barBotLeft = { min.x - barWidth, min.y + maxHeight };


            ImVec2 healthBotLeft = { barBotLeft.x + 1, barBotLeft.y };
            float healthHeight = (maxHeight - 2) * healthRatio;
            ImVec2 healthTopRight = { barTopRight.x - 1, healthBotLeft.y - healthHeight - 2.0f };
            ImVec4 healthColor = { (std::min)((2.0f * (100 - (healthRatio * 100.0f))) / 100.0f, 1.0f),
                (std::min)((2.0f * (healthRatio * 100.0f)) / 100.0f, 1.0f),
                0.0f, 1.0f };

            ImGui::GetBackgroundDrawList()->AddRect(barBotLeft, barTopRight, IM_COL32_BLACK); //Draw healthbar outline
            ImGui::GetBackgroundDrawList()->AddRectFilled(healthBotLeft, healthTopRight, ImGui::GetColorU32(healthColor)); //Draw healthbar

            if (espSettings->healthBarText && !(healthRatio == 1.0f && !espSettings->healthBarTextFull))
                RenderESPText(ImVec2(healthTopRight.x - (espSettings->healthBarThickness / 2), healthTopRight.y), ImGui::GetColorU32(espSettings->healthBarTextColor), ImGui::GetColorU32(espSettings->healthBarTextOutlineColor), enemyHealth, true, true);
        }

        if (espSettings->showInfo)
        {

            std::string enemyName = "";
            if (espSettings->showName)
            {
                enemyName = enemyInfo->enemyObjectName;
                auto gPrefabIndex = enemyName.find("GeneratedPrefab");
                if (gPrefabIndex != std::string::npos)
                {
                    enemyName = enemyName.substr(0, gPrefabIndex);
                    std::replace(enemyName.begin(), enemyName.end(), '_', ' ');
                    enemyName.erase(enemyName.find_last_not_of(' ') + 1);
                }
            }

            std::string enemyType = "";
            if (espSettings->showType)
            {
                switch (enemyInfo->enemyAgent->fields.EnemyData->fields._EnemyType_k__BackingField)
                {
                case app::eEnemyType__Enum::Weakling:
                    enemyType = "Weakling";
                    break;
                case app::eEnemyType__Enum::Standard:
                    enemyType = "Standard";
                    break;
                case app::eEnemyType__Enum::Special:
                    enemyType = "Special";
                    break;
                case app::eEnemyType__Enum::MiniBoss:
                    enemyType = "MiniBoss";
                    break;
                case app::eEnemyType__Enum::Boss:
                    enemyType = "Boss";
                    break;
                }
            }

            ImU32 color = ImGui::GetColorU32(espSettings->textColor);
            ImU32 outlineColor = ImGui::GetColorU32(espSettings->textOutlineColor);

            float center = (min.x + max.x) / 2.0f;
            ImVec2 topCenter = { center, min.y };
            ImVec2 bottomCenter = { center, max.y };
            ImVec2 typeTextSize = ImVec2();
            if (espSettings->showType)
                typeTextSize = RenderESPText(topCenter, color, outlineColor, enemyType, true, true);
            if (espSettings->showName)
                RenderESPText(ImVec2(topCenter.x, topCenter.y - typeTextSize.y), color, outlineColor, enemyName, true, true);


            ImVec2 distanceTextSize = ImVec2();
            if (espSettings->showDistance)
                distanceTextSize = RenderESPText(bottomCenter, color, outlineColor, "[" + std::to_string(llround(enemyInfo->distance)) + "m]", true, false);
            if (espSettings->showHealth)
                RenderESPText(ImVec2(bottomCenter.x, bottomCenter.y + distanceTextSize.y), color, outlineColor, "HP: " + enemyHealth, true, false);

        }
        DrawSkeleton(enemyInfo);
    }

    ImVec2 RenderESPText(ImVec2 drawPos, ImU32 color, ImU32 outlineColor, std::string text, bool centered, bool swapHeight)
    {
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        float horizOffset = centered ? textSize.x / 2 : 0.0f;
        float vertOffset = swapHeight ? textSize.y : 0.0f;
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(drawPos.x - horizOffset + 1, drawPos.y - vertOffset + 1), outlineColor, text.c_str());
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(drawPos.x - horizOffset - 1, drawPos.y - vertOffset - 1), outlineColor, text.c_str());
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(drawPos.x - horizOffset + 1, drawPos.y - vertOffset - 1), outlineColor, text.c_str());
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(drawPos.x - horizOffset - 1, drawPos.y - vertOffset + 1), outlineColor, text.c_str());
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(drawPos.x - horizOffset, drawPos.y - vertOffset), color, text.c_str());

        return textSize;
    }

    void DrawSkeleton(Enemy::EnemyInfo* fullInfo)
    {
        for (std::vector<std::vector<app::HumanBodyBones__Enum>>::iterator itOuter = Enemy::skeletonBones.begin(); itOuter != Enemy::skeletonBones.end(); ++itOuter)
        {
            Enemy::Bone prevBone;
            for (std::vector<app::HumanBodyBones__Enum>::iterator it = (*itOuter).begin(); it != (*itOuter).end(); ++it)
            {
                Enemy::Bone curBone = fullInfo->skeletonBones[*it];
                if (prevBone.position.x == 0.0f && prevBone.position.y == 0.0f && prevBone.position.z == 0.0f)
                {
                    prevBone = curBone;
                    continue;
                }
                if (curBone.position.x == 0.0f && curBone.position.y == 0.0f && curBone.position.z == 0.0f)
                    continue;

                if (prevBone.visible || curBone.visible)
                {
                    if (!ESP::enemyESP.visibleSec.showSkeleton || fullInfo->distance > ESP::enemyESP.visibleSec.skeletonRenderDistance)
                        continue;
                    DrawBone(ImGui::GetColorU32(ESP::enemyESP.visibleSec.skeletonColor), prevBone.position, curBone.position, ESP::enemyESP.visibleSec.skeletonThickness);
                }
                else
                {
                    if (!ESP::enemyESP.nonVisibleSec.showSkeleton || fullInfo->distance > ESP::enemyESP.nonVisibleSec.skeletonRenderDistance)
                        continue;
                    DrawBone(ImGui::GetColorU32(ESP::enemyESP.nonVisibleSec.skeletonColor), prevBone.position, curBone.position, ESP::enemyESP.nonVisibleSec.skeletonThickness);
                }

                prevBone = curBone;
            }
        }
    }

    void DrawBone(ImU32 color, app::Vector3 startBone, app::Vector3 endBone, float thickness)
    {
        if (!G::mainCamera)
            return;
        ImVec2 drawPosStart, drawPosEnd;
        if (!Math::WorldToScreen(startBone, drawPosStart) || !Math::WorldToScreen(endBone, drawPosEnd))
            return;
        ImGui::GetBackgroundDrawList()->AddLine(drawPosStart, drawPosEnd, color, thickness);
    }
}