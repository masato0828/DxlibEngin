#pragma once
#include "../Common/Geometry.h"
#include <string>
#include "../../imGui/imgui.h"

namespace ImGui
{
    

    // ImGui::InputText() with std::string
    // Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
    IMGUI_API bool  DragFloat3(const char* label, Vector3* vec3, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool  DragFloat2(const char* label, Vector2Flt* vec2, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

    //IMGUI_API bool InputText(const char* label, std::string* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
}

namespace ImGuiCustom
{
    struct IM_COLOR
    {
        float red;
        float green;
        float blue;
        float alpha;
    };

    void CreateDokingArea(std::string areaName);

    // 真ん中のドッキングをさせないようにする
    void NoCenterDoking();
    void RenderCustomTitleBar(std::string windowName);

    void HelpMarker(const char* desc);

    IMGUI_API bool  ColorEdit3(const char* label, IM_COLOR* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit4(const char* label, IM_COLOR* col, ImGuiColorEditFlags flags = 0);
}
