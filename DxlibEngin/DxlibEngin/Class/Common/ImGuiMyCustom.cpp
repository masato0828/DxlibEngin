#include "../../imGui/imgui.h"
#include "../../imGui/imgui_internal.h"
#include "ImGuiMyCustom.h"

IMGUI_API bool ImGui::DragFloat3(const char* label, Vector3* vec3, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return DragScalarN(label, ImGuiDataType_Float, vec3, 3, v_speed, &v_min, &v_max, format, flags);
}

IMGUI_API bool ImGui::DragInt2(const char* label, Vector2* vec2, int v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
{
    return DragScalarN(label, ImGuiDataType_S32, vec2, 2, v_speed, &v_min, &v_max, format, flags);
}

IMGUI_API bool ImGui::DragFloat2(const char* label, Vector2Flt* vec2, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return DragScalarN(label, ImGuiDataType_Float, vec2, 2, v_speed, &v_min, &v_max, format, flags);
}

IMGUI_API bool ImGui::DragDouble2(const char* label, Vector2Dbl* vec2, double v_speed, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
{
    return DragScalarN(label, ImGuiDataType_Double, vec2, 2, v_speed, &v_min, &v_max, format, flags);
}

IMGUI_API bool ImGuiCustom::ColorEdit3(const char* label, IM_COLOR* col, ImGuiColorEditFlags flags)
{
    float color[4] = {col->red,col->green ,col->blue,col->alpha};
    bool flg = ImGui::ColorEdit4(label, color, flags | ImGuiColorEditFlags_NoAlpha);
    col->red =  color[0];
    col->green =  color[1];
    col->blue =  color[2];
    col->alpha =  color[3];
	return flg;
}
IMGUI_API bool ImGuiCustom::ColorEdit4(const char* label, IM_COLOR* col, ImGuiColorEditFlags flags)
{
    float color[4] = { col->red,col->green ,col->blue,col->alpha };
    bool flg = ImGui::ColorEdit4(label, color, flags);
    col->red = color[0];
    col->green = color[1];
    col->blue = color[2];
    col->alpha = color[3];
    return flg;
}

void ImGuiCustom::SetCustomStyle(ImGuiCol colType, ImVec4 colorStyle)
{
    // カスタムスタイルの設定
    ImGuiStyle& style = ImGui::GetStyle();
    // 各色の変更
    style.Colors[colType] = colorStyle;
}

void ImGuiCustom::NoCenterDoking()
{
	// 真ん中のドッキングをさせないようにする
	if (ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(ImGui::GetWindowDockID()))
	{
		dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingOverMe;
	}
}

void ImGuiCustom::HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
   
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
