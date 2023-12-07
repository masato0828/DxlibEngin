#include "../../imGui/imgui.h"
#include "../../imGui/imgui_internal.h"
#include "ImGuiMyCustom.h"
#include "Utility.h"

static inline ImGuiInputTextFlags InputScalar_DefaultCharsFilter(ImGuiDataType data_type, const char* format)
{
    if (data_type == ImGuiDataType_Float || data_type == ImGuiDataType_Double)
        return ImGuiInputTextFlags_CharsScientific;
    const char format_last_char = format[0] ? format[strlen(format) - 1] : 0;
    if ((format_last_char == 'x' || format_last_char == 'X')) {
        return ImGuiInputTextFlags_CharsHexadecimal;
    }
    return ImGuiInputTextFlags_CharsDecimal;
}

// Getter for the old Combo() API: const char*[]
static bool Items_wArrayGetter(void* data, int idx, const wchar_t** out_text)
{
    const wchar_t* const* items = (const wchar_t* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}

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
IMGUI_API bool ImGuiCustom::ColorEdit3(const char* label, COLOR_F* col, ImGuiColorEditFlags flags)
{
    float color[4] = { col->r,col->g ,col->b,col->a };
    bool flg = ImGui::ColorEdit4(label, color, flags | ImGuiColorEditFlags_NoAlpha);
    col->r = color[0];
    col->g = color[1];
    col->b = color[2];
    col->a = color[3];
    return flg;
}
IMGUI_API bool ImGuiCustom::ColorEdit3(const char* label, FLOAT4* col, ImGuiColorEditFlags flags)
{
    float color[4] = { col->x,col->y ,col->z,col->w };
    bool flg = ImGui::ColorEdit4(label, color, flags | ImGuiColorEditFlags_NoAlpha);
    col->x = color[0];
    col->y = color[1];
    col->z = color[2];
    col->w = color[3];
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

IMGUI_API bool ImGuiCustom::ColorEdit4(const char* label, COLOR_F* col, ImGuiColorEditFlags flags)
{
    float color[4] = { col->r,col->g ,col->b,col->a };
    bool flg = ImGui::ColorEdit4(label,color, flags);
    col->r = color[0];
    col->g = color[1];
    col->b = color[2];
    col->a = color[3];
    return flg;
}

IMGUI_API bool ImGuiCustom::ColorEdit4(const char* label, FLOAT4* col, ImGuiColorEditFlags flags)
{
    float color[4] = { col->x,col->y ,col->z,col->w };
    bool flg = ImGui::ColorEdit4(label, color, flags);
    col->x = color[0];
    col->y = color[1];
    col->z = color[2];
    col->w = color[3];
    return flg;
}

IMGUI_API bool ImGuiCustom::InputFloat3(const char* label, Vector3* vec3, float step, float step_fast, const char* format, ImGuiInputTextFlags flags)
{
    ImGui::InputFloat(std::string(label + std::string("x")).c_str(), &vec3->x_, step, step_fast, format, flags);
    ImGui::InputFloat(std::string(label + std::string("y")).c_str(), &vec3->y_, step, step_fast, format, flags);
    ImGui::InputFloat(std::string(label + std::string("z")).c_str(), &vec3->z_, step, step_fast, format, flags);
    return false;
}

IMGUI_API bool ImGuiCustom::InputFloat3InText(const char* label, Vector3* vec3, float step, float step_fast, const char* format, ImGuiInputTextFlags flags)
{
    ImGui::InputFloat(std::string(label + std::string("x")).c_str(), &vec3->x_, step, step_fast, format, flags);
    ImGui::SameLine();
    ImGui::Text(std::string(label + std::string("x:%f")).c_str(), Utility::Rad2DegF(vec3->x_));

    ImGui::InputFloat(std::string(label + std::string("y")).c_str(), &vec3->y_, step, step_fast, format, flags);
    ImGui::SameLine();
    ImGui::Text(std::string(label + std::string("y:%f")).c_str(), Utility::Rad2DegF(vec3->y_));

    ImGui::InputFloat(std::string(label + std::string("z")).c_str(), &vec3->z_, step, step_fast, format, flags);
    ImGui::SameLine();
    ImGui::Text(std::string(label + std::string("z:%f")).c_str(), Utility::Rad2DegF(vec3->z_));
    return false;
}

IMGUI_API bool ImGuiCustom::InputFloat3CameraMove(const char* label, Vector3* angle, Vector3* vec3, float step, float step_fast, const char* format, ImGuiInputTextFlags flags)
{
    auto text = reinterpret_cast<const char*>(u8"左右移動");
    ImGui::Text(text); ImGui::SameLine();
    InputScalar(std::string(std::string("##左右移動") + label).c_str(), ImGuiDataType_Float, &vec3->x_, &vec3->z_, angle->y_ + Utility::Deg2Rad(90.0f), (void*)(step > 0.0f ? &step : NULL), (void*)(step_fast > 0.0f ? &step_fast : NULL));
    text = reinterpret_cast<const char*>(u8"前後移動");
    ImGui::Text(text); ImGui::SameLine();
    InputScalar(std::string(std::string("##前後移動") + label).c_str(), ImGuiDataType_Float, &vec3->x_, &vec3->z_, angle->y_, (void*)(step > 0.0f ? &step : NULL), (void*)(step_fast > 0.0f ? &step_fast : NULL));
    return false;
}

IMGUI_API bool ImGuiCustom::InputScalar(const char* label, ImGuiDataType data_type, void* p_dataX, void* p_dataZ, float angleData, const void* p_step, const void* p_step_fast, const char* format, ImGuiInputTextFlags flags)
{
    float rad = 0.0f;

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;

    if (format == NULL)
        format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

    char buf[64];
    ImGui::DataTypeFormatString(buf, IM_ARRAYSIZE(buf), data_type, p_dataX, format);

    // Testing ActiveId as a minor optimization as filtering is not needed until active
    if (g.ActiveId == 0 && (flags & (ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsScientific)) == 0)
        flags |= InputScalar_DefaultCharsFilter(data_type, format);
    flags |= ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoMarkEdited; // We call MarkItemEdited() ourselves by comparing the actual data rather than the string.

    bool value_changed = false;
    if (p_step != NULL)
    {
        const float button_size = ImGui::GetFrameHeight();

        ImGui::BeginGroup(); // The only purpose of the group here is to allow the caller to query item data e.g. IsItemActive()
        ImGui::PushID(label);
        ImGui::SetNextItemWidth(ImMax(1.0f, ImGui::CalcItemWidth() - (button_size + style.ItemInnerSpacing.x) * 2));
        //if (ImGui::InputText("", buf, IM_ARRAYSIZE(buf), flags)) // PushId(label) + "" gives us the expected ID from outside point of view
        //    value_changed = ImGui::DataTypeApplyFromText(buf, data_type, p_dataX, format);
        IMGUI_TEST_ENGINE_ITEM_INFO(g.LastItemData.ID, label, g.LastItemData.StatusFlags);

        // Step buttons
        const ImVec2 backup_frame_padding = style.FramePadding;
        style.FramePadding.x = style.FramePadding.y;
        ImGuiButtonFlags button_flags = ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups;
        if (flags & ImGuiInputTextFlags_ReadOnly)
            ImGui::BeginDisabled();
        //ImGui::SameLine(0, style.ItemInnerSpacing.x);
        if (ImGui::ButtonEx("-", ImVec2(button_size, button_size), button_flags))
        {
            auto rad = (angleData * (DX_PI_F / 180.0f));
            (*(float*)p_dataX) -= sinf(angleData) * *(const float*)p_step;
            (*(float*)p_dataZ) -= cosf(angleData) * *(const float*)p_step;

            value_changed = true;
        }
        ImGui::SameLine(0, style.ItemInnerSpacing.x);
        if (ImGui::ButtonEx("+", ImVec2(button_size, button_size), button_flags))
        {
            (*(float*)p_dataX) += sinf(angleData) * *(const float*)p_step;
            (*(float*)p_dataZ) += cosf(angleData) * *(const float*)p_step;

            value_changed = true;
        }
        if (flags & ImGuiInputTextFlags_ReadOnly)
            ImGui::EndDisabled();

        const char* label_end = ImGui::FindRenderedTextEnd(label);
        if (label != label_end)
        {
            ImGui::SameLine(0, style.ItemInnerSpacing.x);
            ImGui::TextEx(label, label_end);
        }
        style.FramePadding = backup_frame_padding;
        ImGui::PopID();
        ImGui::EndGroup();
    }
    else
    {
        if (ImGui::InputText(label, buf, IM_ARRAYSIZE(buf), flags))
            value_changed = ImGui::DataTypeApplyFromText(buf, data_type, p_dataX, format);
    }
    if (value_changed)
        ImGui::MarkItemEdited(g.LastItemData.ID);

    return value_changed;
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

bool ImGui::Begin(const char* name, bool* p_open, WindowZOrder z_order, ImGuiWindowFlags flags)
{
    bool result = ImGui::Begin(name, p_open, flags);
    ImGui::GetCurrentWindow()->BeginOrderWithinContext = z_order;

    return result;
}
