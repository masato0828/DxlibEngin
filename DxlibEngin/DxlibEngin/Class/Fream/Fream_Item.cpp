#include "Fream_Item.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_internal.h"

Fream_Item::Fream_Item()
{
	Init();
}

Fream_Item::~Fream_Item()
{
}

void Fream_Item::Init()
{
}

void Fream_Item::Update()
{
	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_classview;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_classview.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
	ImGui::SetNextWindowClass(&window_classview);
	ImGui::Begin("Items");
	
	ImGui::End();
}
