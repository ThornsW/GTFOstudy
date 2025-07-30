#pragma once
#include "../../../hack/esp.h"
#include "../../../hack/globals.h"
#include "../../../hack/enemy.h"
#include "../../../hack/math.h"
#include <algorithm>

namespace Menu {
	void Init();
	void Render();
	void RenderESP();
	void RenderEnemyAgent(Enemy::EnemyInfo* enemyInfo, ESP::AgentESPSection* espSettings);
	ImVec2 RenderESPText(ImVec2 drawPos, ImU32 color, ImU32 outlineColor, std::string text, bool centered = true, bool swapHeight = false);
	void DrawSkeleton(Enemy::EnemyInfo* fullInfo);
	void DrawBone(ImU32 color, app::Vector3 startBone, app::Vector3 endBone, float thickness);
}