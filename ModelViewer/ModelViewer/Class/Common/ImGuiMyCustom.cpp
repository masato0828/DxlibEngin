#include "../../imGui/imgui.h"
#include "ImGuiMyCustom.h"

IMGUI_API bool ImGui::DragFloat3(const char* label, Vector3* vec3, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalarN(label, ImGuiDataType_Float, vec3, 3, v_speed, &v_min, &v_max, format, flags);
}

IMGUI_API bool ImGui::DragFloat2(const char* label, Vector2Flt* vec2, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalarN(label, ImGuiDataType_Float, vec2, 2, v_speed, &v_min, &v_max, format, flags);;
}

IMGUI_API bool ImGui::ColorEdit3(std::string label, float col[3], ImGuiColorEditFlags flags)
{
	const char* name = label.c_str();
	return ImGui::ColorEdit3(name, col, flags);
}
//
//IMGUI_API bool ImGui::InputText(const char* label, std::string* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
//{
//
//	return ImGui::InputText(label, buf, buf_size, flags, callback, user_data);
//}


