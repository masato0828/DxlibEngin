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

	// ���[����ǉ����邽�߂̐��K�\���ƐF�̃C���f�b�N�X�̃y�A���쐬
	TextEditor::LanguageDefinition::TokenRegexStrings customRegexStrings;

	//// �L�[���[�h�̃��[����ǉ�
	customRegexStrings.push_back(std::make_pair("\\b(if|else|while|for)\\b", TextEditor::PaletteIndex::Custom2_Include));

	// �J�X�^���̃V���^�b�N�X�n�C���C�g���[�����e�L�X�g�G�f�B�^�ɐݒ�
	TextEditor::LanguageDefinition customLanguage = editor.GetLanguageDefinition();
	customLanguage.mTokenRegexStrings = customRegexStrings;

	// �֐����̐F��ݒ�
	TextEditor::Palette customPalette = editor.GetDarkPalette();
	customPalette[(unsigned)TextEditor::PaletteIndex::Custom1_Function] = 0xff7070e0;		// 
	customPalette[(unsigned)TextEditor::PaletteIndex::MultiLineComment] = 0xffaaffaa;
	customPalette[(unsigned)TextEditor::PaletteIndex::Comment] = 0xffaaffaa;
	// �p���b�g���e�L�X�g�G�f�B�^�ɐݒ�
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

	// Imgui�p�E�B���h�E�N���X�̍쐬
	ImGuiWindowClass window_classview;
	// �E�B���h�E�̌��ʂ̕ҏW(�E�B���h�E�̃h�b�L���O�͐؂��Ă���)
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

	// load�{�^�����������ꍇ
	if (ImGui::Button("Load"))
	{
		isText_ = true;
	}

	// �O��ǂݍ��񂾃R�[�h�ƈႤ�ꍇ
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

				// �֐����̐F��ݒ�
				TextEditor::Palette customPalette = editor.GetDarkPalette();
				customPalette[(unsigned)TextEditor::PaletteIndex::Custom1_Function] = 0xff7070e0;
				// �p���b�g���e�L�X�g�G�f�B�^�ɐݒ�
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
	//	// �e�L�X�g�G�f�B�^�̓��e��K�p���āAlines�x�N�^�[���X�V����
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
