#include "Fream_FileDialog_CodeEditer.h"
#include "../../../imGui/imgui.h"
#include "../../../imGui/imgui_internal.h"
#include "../../Common/Utility.h"

#include <sstream>
#include <filesystem>
#include <fstream>

Fream_FileDialog_CodeEditer::Fream_FileDialog_CodeEditer()
{
	Init();
}

Fream_FileDialog_CodeEditer::~Fream_FileDialog_CodeEditer()
{
}

void Fream_FileDialog_CodeEditer::Init()
{
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

	isText_ = false;
}

void Fream_FileDialog_CodeEditer::Update()
{
}

void Fream_FileDialog_CodeEditer::Update(std::filesystem::path filePath)
{
	std::ifstream file(filePath);
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
	ImGui::SetNextWindowSize(ImVec2(500, 700), ImGuiCond_Once);
	ImGui::Begin("Code Editor");

	ImGui::SetWindowFontScale(1.0f);

	// loadボタンを押した場合
	if (ImGui::Button("Load"))
	{
		isText_ = true;
	}

	// 前回読み込んだコードと違う場合
	if (beforCode_ != code)
	{
		isText_ = true;
	}

	if (isText_)
	{

		if (maltbyteEncoding == "UTF-8")
		{
			editor.SetText(code);
		}
		else
		{

			auto fileName = filePath;



			if (Utility::ComparisonExtensionFile(fileName, L"cpp") ||
				Utility::ComparisonExtensionFile(fileName, L"h"))
			{
				auto w_maltbyte = Utility::MultiByteToUnicode(code);
				auto maltbyte = Utility::WStringToUTF8(w_maltbyte);
				editor.SetText(maltbyte);
			}
			else if (Utility::ComparisonExtensionFile(fileName, L"hlsl"))
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
			}
			else
			{
				auto w_maltbyte = Utility::MultiByteToUnicode(code);
				auto maltbyte = Utility::WStringToUTF8(w_maltbyte);
				editor.SetText(maltbyte);
			}

		}

		beforCode_ = code;

		isText_ = false;
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
	ImGui::End();
}
