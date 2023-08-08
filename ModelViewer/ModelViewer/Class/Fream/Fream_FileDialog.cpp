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
#include "FileDialog\TextEditerSetUp.h"

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
	// 選択用マスク
	//selection_mask_ = (1 << 0);
	nowSelect = &fileData_;

	nowSelectFileName_ = L"";

	tree_ = std::make_unique<Fream_FileDialog_Tree>();
	project_ = std::make_unique<Fream_FileDialog_Project>();
	item_ = std::make_unique<Fream_FileDialog_Item>();
	codeEditer_ = std::make_unique<Fream_FileDialog_CodeEditer>();

	fontNum_ = 0;

	is_update_ = false;

	//editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());

	//// ルールを追加するための正規表現と色のインデックスのペアを作成
	//TextEditor::LanguageDefinition::TokenRegexStrings customRegexStrings;

	////// キーワードのルールを追加
	//customRegexStrings.push_back(std::make_pair("\\b(if|else|while|for)\\b", TextEditor::PaletteIndex::Custom2_Include));

	//// カスタムのシンタックスハイライトルールをテキストエディタに設定
	//TextEditor::LanguageDefinition customLanguage = editor.GetLanguageDefinition();
	//customLanguage.mTokenRegexStrings = customRegexStrings;

	//// 関数名の色を設定
	//TextEditor::Palette customPalette = editor.GetDarkPalette();
	//customPalette[(unsigned)TextEditor::PaletteIndex::Custom1_Function] = 0xff7070e0;		// 
	//customPalette[(unsigned)TextEditor::PaletteIndex::MultiLineComment] = 0xffaaffaa;
	//customPalette[(unsigned)TextEditor::PaletteIndex::Comment] = 0xffaaffaa;
	//// パレットをテキストエディタに設定
	//editor.SetPalette(customPalette);

	//editor.SetLanguageDefinition(customLanguage);
	//TextEditerSetUp::SetUp(editor);
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
	
	//EditMainCppCode();
	codeEditer_->Update(fileFullPaht_ /= nowSelectFile_);

	item_->GetButton_Click() = false;
}

std::filesystem::path Fream_FileDialog::GetNowFile()
{
	return fileFullPaht_ /= nowSelectFile_;
}

void Fream_FileDialog::EditMainCppCode()
{
	std::filesystem::path f = fileFullPaht_ /= nowSelectFile_;

	std::ifstream file(f);

	std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	std::string maltbyteEncoding = Utility::DetectMaltbyteEncoding(code);

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

	for (const auto& line : lines)
	{
		ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImGui::TextColored(color, line.c_str());
	}
	item_->GetButton_Click() = false;
	ImGui::End();
}
