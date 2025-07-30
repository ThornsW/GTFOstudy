#pragma once
#include "..\framework\pch-il2cpp.h"
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "..\libraries\imgui\imgui.h"
#include "..\libraries\imgui\imgui_impl_win32.h"
#include "..\libraries\imgui\imgui_impl_dx11.h"
#include <queue>
#include <functional>
#include <mutex>


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef BOOL(WINAPI* hSetCursorPos) (int X, int Y);

namespace G {

    extern std::mutex worldItemsMtx;
    extern std::mutex worldArtifMtx;
    extern std::mutex worldCarryMtx;
    extern std::mutex worldKeyMtx;
    extern std::mutex worldGenericMtx;
    extern std::mutex worldResourcePackMtx;
    extern std::mutex worldTerminalsMtx;
    extern std::mutex worldHSUMtx;
    extern std::mutex worldBulkheadMtx;
    extern std::mutex enemyVecMtx;
    extern std::mutex imguiMtx;
    extern std::mutex enemyAimMtx;

    extern app::Camera* mainCamera;
    extern app::Matrix4x4 w2CamMatrix;
    extern app::Matrix4x4 projMatrix;
    extern app::Matrix4x4 viewMatrix;
    extern app::PlayerAgent* localPlayer;
    extern int32_t screenHeight;
    extern int32_t screenWidth;


    extern std::queue<std::function<void(void)>> callbacks;
}
