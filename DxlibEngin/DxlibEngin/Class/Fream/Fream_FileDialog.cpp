#include "Fream_FileDialog.h"
#include "../../imGui/ImGuiFileDialog.h"
#include "../../imGui/CustomFont.h"
#include "../Common/FileDialog.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"
#include <shlwapi.h>
#include <Windows.h>
#include <string>
#include <shobjidl_core.h >
#include <sstream>

#include "../Common/Utility.h"
#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/ImGuiColorTextEdit/TextEditor.h"

Fream_FileDialog::Fream_FileDialog():fileData_(nullptr, (--std::filesystem::current_path().end())->wstring())
{
	Init();
}

Fream_FileDialog::~Fream_FileDialog()
{
}

void Fream_FileDialog::Init()
{
	// 選択している数
	nowSelect = nullptr;
	// 選択用マスク
	//selection_mask_ = (1 << 0);
	nowSelect = &fileData_;

	nowSelectFileName_ = L"";

	tree_ = std::make_unique<Fream_FileDialog_Tree>();
	project_ = std::make_unique<Fream_FileDialog_Project>();
	item_ = std::make_unique<Fream_FileDialog_Item>();
	codeEditer_ = std::make_unique<Fream_FileDialog_CodeEditer>();
}

void Fream_FileDialog::Update()
{
}

void Fream_FileDialog::Update(bool codeEditerWindowOpenFlg)
{
	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_class;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
	project_->Update();

	ImGui::SetNextWindowClass(&window_class);
	tree_->Update(fileData_,nowSelect,nowSelectPath_,nowSelectFileName_);

	ImGui::SetNextWindowClass(&window_class);
	item_->Update(nowSelect,fileFullPaht_, nowSelectFile_, nowSelectFileName_);
	
	if (codeEditerWindowOpenFlg)
	{
		codeEditer_->Update(fileFullPaht_ /= nowSelectFile_);
	}
	item_->GetButton_Click() = false;
}

std::filesystem::path Fream_FileDialog::GetNowFile()
{
	return fileFullPaht_ /= nowSelectFile_;
}
