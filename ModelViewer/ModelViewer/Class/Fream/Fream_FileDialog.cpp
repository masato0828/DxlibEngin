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

	editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());

	// ルールを追加するための正規表現と色のインデックスのペアを作成
	TextEditor::LanguageDefinition::TokenRegexStrings customRegexStrings;

	//// キーワードのルールを追加
	customRegexStrings.push_back(std::make_pair("\\b(if|else|while|for)\\b", TextEditor::PaletteIndex::Custom2_Include));

	// カスタムのシンタックスハイライトルールをテキストエディタに設定
	TextEditor::LanguageDefinition customLanguage = editor.GetLanguageDefinition();
	customLanguage.mTokenRegexStrings = customRegexStrings;

	// 関数名の色を設定
	TextEditor::Palette customPalette = editor.GetDarkPalette();
	customPalette[(unsigned)TextEditor::PaletteIndex::Custom1_Function] = 0xff7070e0;		// 
	customPalette[(unsigned)TextEditor::PaletteIndex::MultiLineComment] = 0xffaaffaa;
	customPalette[(unsigned)TextEditor::PaletteIndex::Comment] = 0xffaaffaa;
	// パレットをテキストエディタに設定
	editor.SetPalette(customPalette);

	editor.SetLanguageDefinition(customLanguage);
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

	std::ifstream file(fileFullPaht_ /= nowSelectFile_);
	std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::string maltbyteEncoding = Utility::DetectMaltbyteEncoding(code);

	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_classview;
	// ウィンドウの効果の編集(ウィンドウのドッキングは切っておく)
	window_classview.DockNodeFlagsOverrideSet =
		ImGuiDockNodeFlags_NoDocking |
		ImGuiDockNodeFlags_NoDockingSplitMe |
		ImGuiDockNodeFlags_NoDockingSplitOther |
		ImGuiDockNodeFlags_NoDockingOverMe |
		ImGuiDockNodeFlags_NoDockingOverOther |
		ImGuiDockNodeFlags_NoDockingOverEmpty;
	ImGui::SetNextWindowClass(&window_classview);
	ImGui::SetNextWindowSize(ImVec2(500,700),ImGuiCond_Once);
	ImGui::Begin("Code Editor");

	ImGui::SetWindowFontScale(1.0f);

	if (ImGui::Button("Update") || item_->GetButton_Click())
	{
		is_update_ = true;
	}

	if (is_update_)
	{
		if (maltbyteEncoding == "UTF-8")
		{
			editor.SetText(code);
			is_update_ = false;
		}
		else
		{

			auto fileName = fileFullPaht_ /= nowSelectFile_;



			if (Utility::IsHeaderFile(fileName.u8string(), "cpp") ||
				Utility::IsHeaderFile(fileName.u8string(), "h"))
			{
				auto w_maltbyte = Utility::MultiByteToUnicode(code);
				auto maltbyte = Utility::WStringToUTF8(w_maltbyte);
				editor.SetText(maltbyte);
				is_update_ = false;
			}
			else if (Utility::IsHeaderFile(fileName.u8string(), "hlsl"))
			{
				auto w_maltbyte = Utility::MultiByteToUnicode(code);
				auto maltbyte = Utility::WStringToUTF8(w_maltbyte);
				editor.SetLanguageDefinition(TextEditor::LanguageDefinition::HLSL());

				// 関数名の色を設定
				TextEditor::Palette customPalette = editor.GetDarkPalette();
				customPalette[(unsigned)TextEditor::PaletteIndex::Custom1_Function] = 0xff7070e0;
				// パレットをテキストエディタに設定
				editor.SetPalette(customPalette);


				editor.SetText(maltbyte);
				is_update_ = false;
			}
			else
			{
				auto w_maltbyte = Utility::MultiByteToUnicode(code);
				auto maltbyte = Utility::WStringToUTF8(w_maltbyte);
				editor.SetText(maltbyte);
				is_update_ = false;
			}

		}
	}

	editor.Render("##codeEditor", ImVec2(-1, -1));

	//if (ImGui::Button("Apply"))
	//{
	//	// テキストエディタの内容を適用して、linesベクターを更新する
	//	lines.clear();

	//	std::string text = editor.GetText();
	//	std::istringstream iss(text);
	//	std::string line;
	//	while (std::getline(iss, line))
	//	{
	//		lines.push_back(line);
	//	}
	//}

	//for (const auto& line : lines)
	//{
	//	ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	//	ImGui::TextColored(color, line.c_str());
	//}

	item_->GetButton_Click() = false;
	ImGui::End();
}

void Fream_FileDialog::EditMainCppCode()
{
	std::ifstream file(fileFullPaht_ /= nowSelectFile_);

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
