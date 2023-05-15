#pragma once
#include "../Common/Geometry.h"
#include <string>

namespace ImGui
{
    // ImGui::InputText() with std::string
    // Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
    IMGUI_API bool  DragFloat3(const char* label, Vector3* vec3, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool  DragFloat2(const char* label, Vector2Flt* vec2, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

    IMGUI_API bool  ColorEdit3(std::string label, float col[3], ImGuiColorEditFlags flags = 0);

    //IMGUI_API bool InputText(const char* label, std::string* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
}

namespace ImGuiCustom
{
    void CreateDokingArea(std::string areaName);

    // 真ん中のドッキングをさせないようにする
    void NoCenterDoking();
}
