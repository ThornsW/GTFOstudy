#include "globals.h"

namespace G {

    std::mutex worldItemsMtx;
    std::mutex worldArtifMtx;
    std::mutex worldCarryMtx;
    std::mutex worldKeyMtx;
    std::mutex worldGenericMtx;
    std::mutex worldResourcePackMtx;
    std::mutex worldTerminalsMtx;
    std::mutex worldHSUMtx;
    std::mutex worldBulkheadMtx;
    std::mutex enemyVecMtx;
    std::mutex imguiMtx;
    std::mutex enemyAimMtx;

    app::Camera* mainCamera = NULL;
    app::Matrix4x4 w2CamMatrix;
    app::Matrix4x4 projMatrix;
    app::Matrix4x4 viewMatrix;
    app::PlayerAgent* localPlayer = NULL;
    int32_t screenHeight = NULL;
    int32_t screenWidth = NULL;

    std::queue<std::function<void()>> callbacks;
}