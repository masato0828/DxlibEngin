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
	// �I�����Ă��鐔
	nowSelect = nullptr;
	selected_ = 0;
	// �I��p�}�X�N
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
	// Imgui�p�E�B���h�E�N���X�̍쐬
	ImGuiWindowClass window_class;
	// �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
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

	ImGui::SetNextWindowSize(ImVec2(500,700),ImGuiCond_Once);
	ImGui::Begin("Code Editor");

	ImGui::SetWindowFontScale(1.0f);

	if (ImGui::Button("Update") || item_->GetButton_Click())
	{
		is_update_ = true;
	}

	static TextEditor editor;
	editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());

	// ���[����ǉ����邽�߂̐��K�\���ƐF�̃C���f�b�N�X�̃y�A���쐬
	TextEditor::LanguageDefinition::TokenRegexStrings customRegexStrings;

	// ���ʎq�̃��[����ǉ�
	customRegexStrings.push_back(std::make_pair("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b", TextEditor::PaletteIndex::Identifier));

	// �L�[���[�h�̃��[����ǉ�
	customRegexStrings.push_back(std::make_pair("\\b(if|else|while|for)\\b", TextEditor::PaletteIndex::Keyword));

	// �֐����̃��[����ǉ�
	customRegexStrings.push_back(std::make_pair("\\b([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\(", TextEditor::PaletteIndex::Custom1_Function));

	// �C���N���[�h���̃��[����ǉ�
	customRegexStrings.push_back(std::make_pair("#include\\s*<([^>]*)>", TextEditor::PaletteIndex::Identifier));

	// ���̃��[�����ǉ�����ꍇ�́A���l�̕��@�ōs���܂�

	// �J�X�^���̃V���^�b�N�X�n�C���C�g���[�����e�L�X�g�G�f�B�^�ɐݒ�
	TextEditor::LanguageDefinition customLanguage = editor.GetLanguageDefinition();
	customLanguage.mTokenRegexStrings = customRegexStrings;

	// �֐����̐F��ݒ�
	TextEditor::Palette customPalette = editor.GetDarkPalette();
	editor.SetPalette(customPalette);

	editor.SetLanguageDefinition(customLanguage);
	//TextEditerSetUp::SetUp(editor);

	

	if (maltbyteEncoding == "UTF-8")
	{
		if (is_update_)
		{
			editor.SetText(code);
			is_update_ = false;
		}
	}
	else
	{
		if (is_update_)
		{
			auto w_maltbyte = Utility::MultiByteToUnicode(code);
			auto maltbyte = Utility::WStringToUTF8(w_maltbyte);
			editor.SetText(maltbyte);
			is_update_ = false;
		}
	}

	editor.Render("##codeEditor", ImVec2(-1, -1));

	if (ImGui::Button("Apply"))
	{
		// �e�L�X�g�G�f�B�^�̓��e��K�p���āAlines�x�N�^�[���X�V����
		lines.clear();

		std::string text = editor.GetText();
		std::istringstream iss(text);
		std::string line;
		while (std::getline(iss, line))
		{
			lines.push_back(line);
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

			// ���s�����i'\n'�j�ŕ�����𕪊����čs���ƂɃx�N�^�[�Ɋi�[
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

			// ���s�����i'\n'�j�ŕ�����𕪊����čs���ƂɃx�N�^�[�Ɋi�[
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
