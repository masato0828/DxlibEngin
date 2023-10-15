#pragma once
#include "../Common/Geometry.h"
#include <string>
#include <Dxlib.h>
#include "../../imGui/imgui.h"

namespace ImGui
{
    /// <summary>
    /// 3D座標カスタム
    /// </summary>
    /// <param name="label">タイトル</param>
    /// <param name="vec3">3D座標</param>
    /// <param name="v_speed">カスタムスピード</param>
    /// <param name="v_min">ドラッグの最小</param>
    /// <param name="v_max">ドラッグの最大</param>
    /// <param name="format">表示(%.3f)</param>
    /// <param name="flags">ImGuiSliderFlags</param>
    /// <returns>触れている：true 　触れていない：false </returns>
    IMGUI_API bool  DragFloat3(const char* label, Vector3* vec3, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    
    // 2D座標カスタム
    IMGUI_API bool  DragInt2(const char* label, Vector2* vec2, int v_speed = 1, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool  DragFloat2(const char* label, Vector2Flt* vec2, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool  DragDouble2(const char* label, Vector2Dbl* vec2, double v_speed = 1.0, double v_min = 0.0, double v_max = 0.0, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    enum WindowZOrder {
        WindowZOrder_Chat,
        WindowZOrder_Scoreboard,
        WindowZOrder_Menu,
        WindowZOrder_Console,
    };
    bool Begin(const char* name, bool* p_open,WindowZOrder z_order, ImGuiWindowFlags flags);
  
}

// カスタム　処理
namespace ImGuiCustom
{
    struct IM_COLOR
    {
        float red;
        float green;
        float blue;
        float alpha;
    };

    // 色の変更カスタム
    IMGUI_API bool  ColorEdit3(const char* label, IM_COLOR* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit3(const char* label, COLOR_F* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit3(const char* label, FLOAT4* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit4(const char* label, IM_COLOR* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit4(const char* label, COLOR_F* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit4(const char* label, FLOAT4* col, ImGuiColorEditFlags flags = 0);

    /// <summary>
   /// ウィンドウを真ん中へのドッキングを無効にする
   /// </summary>
    void NoCenterDoking();

    /// <summary>
    /// ヘルパーテキスト(?)の作成
    /// </summary>
    /// <param name="desc">テキストとして入力したい文字</param>
    void HelpMarker(const char* desc);

    /// <summary>
    /// ImGuiの各色の変更
    /// </summary>
    /// <param name="colType">ImGuiCol</param>
    /// <param name="colorStyle">色</param>
    void SetCustomStyle(ImGuiCol colType, ImVec4 colorStyle);
}
