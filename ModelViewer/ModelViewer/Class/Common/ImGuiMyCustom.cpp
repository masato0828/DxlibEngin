#include "../../imGui/imgui.h"
#include "../../imGui/imgui_internal.h"
#include "ImGuiMyCustom.h"

IMGUI_API bool ImGui::DragFloat3(const char* label, Vector3* vec3, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return DragScalarN(label, ImGuiDataType_Float, vec3, 3, v_speed, &v_min, &v_max, format, flags);
}

IMGUI_API bool ImGui::DragFloat2(const char* label, Vector2Flt* vec2, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return DragScalarN(label, ImGuiDataType_Float, vec2, 2, v_speed, &v_min, &v_max, format, flags);;
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

void ImGuiCustom::SetCustomButtonStyle(ImGuiCol colType, ImVec4 colorStyle)
{
    // カスタムスタイルの設定
    ImGuiStyle& style = ImGui::GetStyle();
    // 各色の変更
    style.Colors[colType] = colorStyle;
}

void ImGuiCustom::ShowWindowsIconImageButton()
{
    // Windowsの規定アイコンをロードする
    HICON hIcon = LoadIcon(NULL, IDI_APPLICATION);

    // アイコンのサイズを取得する
    ICONINFO iconInfo;
    GetIconInfo(hIcon, &iconInfo);
    int iconWidth = iconInfo.xHotspot * 2;
    int iconHeight = iconInfo.yHotspot * 2;

    // アイコンをDxLibのテクスチャに変換する
    int iconTexture = MakeGraph(iconWidth, iconHeight);
    SetDrawScreen(iconTexture);
    HDC hdc = GetDC(NULL);
    DrawIcon(hdc, 0, 0, hIcon);
    ReleaseDC(NULL, hdc);
    SetDrawScreen(DX_SCREEN_BACK);

    // アイコンのテクスチャを解放する
    //DxLib::DeleteGraph(iconTexture);

    // アイコンを解放する
    //DestroyIcon(hIcon);
}

void ImGuiCustom::CreateDokingArea(std::string areaName)
{
    // くっつけるスペースの作成のための変数
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // ドッキングスペースの送信
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID(areaName.c_str());
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}

void ImGuiCustom::NoCenterDoking()
{
	// 真ん中のドッキングをさせないようにする
	if (ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(ImGui::GetWindowDockID()))
	{
		dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingOverMe;
	}
}

void ImGuiCustom::RenderCustomTitleBar(std::string windowName)
{

    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

        ImGui::Begin(windowName.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar);
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();

        float windowHight = 20.0f;

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + 25.0f), IM_COL32(100, 100, 100, 255)); // タイトルバーの色

        // タイトルバーの描画
        drawList->AddText(ImVec2(windowPos.x + 5.0f, windowPos.y + 2.0f), IM_COL32(255, 255, 255, 255),windowName.c_str());

        ImGui::End();

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
    };
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
