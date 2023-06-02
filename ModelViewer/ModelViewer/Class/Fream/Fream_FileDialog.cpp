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

Fream_FileDialog::Fream_FileDialog():fileData_(nullptr, (--std::filesystem::current_path().end())->u8string())
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
	selected_ = 0;
	// 選択用マスク
	//selection_mask_ = (1 << 0);
	nowSelect = &fileData_;

	nowSelectFileName_ = L"";

	tree_ = std::make_unique<Fream_FileDialog_Tree>();
	project_ = std::make_unique<Fream_FileDialog_Project>();
	item_ = std::make_unique<Fream_FileDialog_Item>();

	fontNum_ = 0;

	is_update_ = false;
}

void Fream_FileDialog::Update()
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
	
	EditMainCppCode();
}

void Fream_FileDialog::EditMainCppCode()
{
	std::ifstream file(fileFullPaht_ /= nowSelectFile_);

	std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	std::string maltbyteEncoding = Utility::DetectMaltbyteEncoding(code);

	//ImGuiIO& io = ImGui::GetIO();
	//ImFont* font_current = io.Fonts->Fonts[1];
	//ImGui::PushFont(font_current);  // ウィンドウ内で使用するフォントを設定
	//ImGui::PopFont();
	
	

	ImGui::Begin("Code Editor");

	ImGui::SetWindowFontScale(1.0f);

	if (ImGui::Button("update")|| item_->GetButton_Click())
	{
		is_update_ = true;
	}

	if (maltbyteEncoding == "UTF-8")
	{
		if (is_update_)
		{
			lines.clear();

			// 改行文字（'\n'）で文字列を分割して行ごとにベクターに格納
			std::string line;

			std::istringstream iss(code);
			while (std::getline(iss, line))
			{
				lines.push_back(line);
			}

			is_update_ = false;
		}
	}
	else
	{
		if (is_update_)
		{
			lines.clear();

			// 改行文字（'\n'）で文字列を分割して行ごとにベクターに格納
			std::string line;

			auto w_maltbyte = Utility::MultiByteToUnicode(code);

			auto maltbyte = Utility::WStringToUTF8(w_maltbyte);

			std::istringstream iss(maltbyte);
			while (std::getline(iss, line))
			{
				lines.push_back(line);
			}

			is_update_ = false;
		}
	}

	for (const auto& line : lines)//aaaaaaaaaaaaaaaaa
	{
		ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImGui::TextColored(color, line.c_str());
	}
	item_->GetButton_Click() = false;
	ImGui::End();
}
