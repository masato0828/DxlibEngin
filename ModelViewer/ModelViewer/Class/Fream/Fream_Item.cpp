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
	// Imgui�p�E�B���h�E�N���X�̍쐬
	ImGuiWindowClass window_classview;
	// �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
	window_classview.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
	ImGui::SetNextWindowClass(&window_classview);
	ImGui::Begin("Items");
	
	ImGui::End();
}
