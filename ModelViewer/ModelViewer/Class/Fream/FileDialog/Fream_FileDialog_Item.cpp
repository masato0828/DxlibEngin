#include "Fream_FileDialog_Item.h"
#include "../../../imGui/imgui.h"
#include "../../../imGui/ImGuiFileDialog.h"
#include "../../../imGui/CustomFont.h"
#include "../../Common/FileDialog.h"
#include "../../../imGui/imgui_internal.h"
#include "../../Common/ImGuiMyCustom.h"
#include "../../Common/Utility.h"

Fream_FileDialog_Item::Fream_FileDialog_Item()
{
	Init();
}

Fream_FileDialog_Item::~Fream_FileDialog_Item()
{
}

void Fream_FileDialog_Item::Init()
{
	button_click_ = false;
	contextMenuFlg_ = false;
	context_renameFlg_ = false;
	fileImageShaderDatas_.emplace("non",nullptr);
	fileImageShaderDatas_.emplace("cpp",nullptr);

	int textureSizeX;
	int textureSizeY;
	ImguiSup::LoadBackGroundTextureFromFile(&fileImageShaderDatas_.at("non"),&textureSizeX,&textureSizeY);

	ImguiSup::LoadTextureFromFile("data/iconData/PNG�A�C�R��.png", &fileImageShaderDatas_.at("cpp"), &textureSizeX, &textureSizeY);

}
void Fream_FileDialog_Item::Update()
{
}

void Fream_FileDialog_Item::Update(FileData*& nowselect,std::filesystem::path& fileFullPath, std::wstring& nowSelectFile, std::wstring& nowSelectFileName)
{
	nowSelect_ = nowselect;
	fileFullPaht_ = fileFullPath;
	nowSelectFile_ = nowSelectFile;
	nowSelectFileName_ = nowSelectFileName;

	DokingWindow();

	FileNameWindow();

	// Imgui�p�E�B���h�E�N���X�̍쐬
	ImGuiWindowClass window_class;
	// �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);

	//// �J�X�^���X�^�C���̐ݒ�
	//ImGuiStyle& style = ImGui::GetStyle();
	//ImVec4 originalButtonBgColor = style.Colors[ImGuiCol_Button];
	//style.Colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 0); // �{�^���̔w�i�F�𓧖��ɂ���
	//style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.5f, 0.5f, 0);
	//style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1, 1, 1, 1);
	////style.Colors[ImGuiCol_Border] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // �{�^���̃t���[�����D�F�ɂ���

	

	if (ImGui::Begin("ITEM"))
	{
		//ImGui::Separator();
		if (nowSelect_ != nullptr)
		{
			FileLogWindow();

			auto current_path = std::filesystem::current_path();

			// �C�e���[�^�̍쐬
			std::filesystem::directory_iterator itr(fileFullPaht_);
			std::filesystem::directory_iterator end{};
			std::wstring directoryName;

			auto copySelectPath = nowSelectFileName_;
			
			for (; itr != end; ++itr)
			{
				directoryName = itr->path().filename();

				MakeFileImage(directoryName/*���݃t�H���_�̉��̊K�w�̃t�@�C���y�уt�H���_�̖��O*/);

				if (nowSelectFileName_ != copySelectPath)
				{
					fileFullPaht_ /= nowSelectFileName_;
					copySelectPath = nowSelectFileName_;

					// �t�H���_�̏ꍇ
					if (std::filesystem::is_directory(fileFullPaht_))
					{
						directoryName = fileFullPaht_.filename();

						nowSelect_->fileMap_.try_emplace(Utility::WideStringToString(directoryName), FileData(nowSelect_, Utility::WideStringToString(directoryName)));
						nowSelect_ = &nowSelect_->fileMap_.at(Utility::WideStringToString(directoryName));
					}
					// �t�@�C���̏ꍇ
					else
					{
						Utility::OpenWithDefaultApplication(fileFullPaht_.wstring());
					}
				}
			}
		}
		
		if (contextMenuFlg_)
		{
			// �{�^����ŉE�N���b�N���ꂽ�Ƃ��̏���
			ImGui::OpenPopup("ContextMenu");

			Popup();
		}

		if (context_renameFlg_)
		{
			RenameWindow();
		}


		ImGui::End();
	}

	
	fileFullPath = fileFullPaht_;
	nowSelectFile = nowSelectFile_;
	nowSelectFileName = nowSelectFileName_;
	nowselect = nowSelect_;
}

void Fream_FileDialog_Item::DokingWindow()
{
	if (ImGui::Begin("##ItemWindow"))
	{
		// �����h�b�L���O�ݒ�
		ImGuiID dockspace_id = ImGui::GetID("ItemDock");
		if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
		{
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

			ImGuiID dock_main_id = dockspace_id;
			ImGuiID dock_id_FileDialog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.01, NULL, &dock_main_id);
			ImGuiID dock_id_FileLog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.01, NULL, &dock_main_id);

			ImGui::DockBuilderSetNodeSize(dock_main_id, ImVec2(300, 400));
			ImGui::DockBuilderSetNodeSize(dock_id_FileDialog, ImVec2(ImGui::GetWindowWidth(), ImGui::GetTextLineHeight()*2));
			ImGui::DockBuilderSetNodeSize(dock_id_FileLog, ImVec2(ImGui::GetWindowWidth(), ImGui::GetTextLineHeight()+10));

			ImGui::DockBuilderDockWindow("FileNameLog", dock_id_FileDialog);
			ImGui::DockBuilderDockWindow("FileLog", dock_id_FileLog);
			ImGui::DockBuilderDockWindow("ITEM", dock_main_id);

			ImGui::DockBuilderFinish(dockspace_id);
		}
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		ImGui::End();
	}
}

void Fream_FileDialog_Item::FileNameWindow()
{
	// Imgui�p�E�B���h�E�N���X�̍쐬
	ImGuiWindowClass window_class;
	// �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar| ImGuiDockNodeFlags_NoResizeFlagsMask_;
	ImGui::SetNextWindowClass(&window_class);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	if (ImGui::Begin("FileNameLog"))
	{
		// �E�B���h�E�̔w�i�F��ݒ�
		ImGui::Text(Utility::WStringToUTF8(nowSelectFile_).c_str());
		ImGui::End();
	}
	ImGui::PopStyleColor();
}

void Fream_FileDialog_Item::FileLogWindow()
{
	// Imgui�p�E�B���h�E�N���X�̍쐬
	ImGuiWindowClass window_class;
	// �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
	window_class.DockNodeFlagsOverrideSet = 
		ImGuiDockNodeFlags_NoWindowMenuButton |
		ImGuiDockNodeFlags_NoTabBar | 
		ImGuiDockNodeFlags_NoResizeFlagsMask_
		;
	ImGui::SetNextWindowClass(&window_class);
	ImGui::Begin("FileLog",nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

	ImGui::Separator();
	fileFullPaht_.clear();
	Recovery(nowSelect_);
	fileFullPaht_ = fileFullPaht_.empty() ? std::filesystem::current_path() :
		std::filesystem::current_path() /= fileFullPaht_;

	ImGui::End();
}

void Fream_FileDialog_Item::Recovery(FileData* selectData)
{
	if (selectData->parentFile_ != NULL)
	{
		Recovery(selectData->parentFile_);
		fileFullPaht_ += selectData->myName + "\\";
		ImGui::SameLine();
		ImGui::SmallButton("/");
	}
	ImGui::SameLine();
	// �{�^���̃T�C�Y�ƕ����̔z�u��ݒ肷��
	if (ImGui::SmallButton(selectData->myName.c_str()))
	{
		nowSelect_ = selectData;
		return;
	}

}

void Fream_FileDialog_Item::MakeFileImage(
	std::wstring name)
{
	ImVec2 buttonPos = ImGui::GetCursorPos();
	ImVec2 buttonSize = ImVec2(80, 80);
	float buttonSpacing = 10.0f; // �{�^���Ԃ̃X�y�[�X
	
	ImGui::BeginGroup();
	{
		// ���j�[�N��ID���쐬
		ImGuiID buttonID = ImGui::GetID(name.c_str());

		// �{�^���𐶐�
		ImGui::PushID(buttonID);
		bool buttonPressed;
		
		FileAssignments(name, buttonPressed, {buttonSize.x,buttonSize.y});
		
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
		{
			// �{�^�����_�u���N���b�N���ꂽ�Ƃ��̏���
			nowSelectFileName_ = name.c_str();
		}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
		{
			if (nowSelectFile_ != name.c_str())
			{
				button_click_ = true;
			}
			// �{�^���������ꂽ�Ƃ��̏���
			nowSelectFile_ = name.c_str();
			
		}
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
		{
			fileName_ = name.c_str();
			contextMenuFlg_ = true;
		}

		// ���̏ꏊ���N���b�N������R���e�L�X�g���j���[�����
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			if (!ImGui::IsAnyItemHovered() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			{
				contextMenuFlg_ = false;
			}
		}

		ImGui::PopID();

		

		// �e�L�X�g�̕����v�Z
		std::wstring fileName = name;
		ImVec2 textSize = ImGui::CalcTextSize(Utility::WStringToUTF8(fileName).c_str());

		// �{�^���̕��ƕ�����̒������r���A�{�^���̕��𒴂���ꍇ�͕������؂�l�߂� "..." ��ǉ�
		if (textSize.x > buttonSize.x)
		{
			while (textSize.x > buttonSize.x && fileName.length() > 0)
			{
				fileName.pop_back();
				// �e�L�X�g�̃T�C�Y�𖈉�X�V����
				textSize = ImGui::CalcTextSize(Utility::WStringToUTF8(fileName).c_str());
			}
			fileName += L"...";
			// ������̃T�C�Y���擾
			textSize = ImGui::CalcTextSize(Utility::WStringToUTF8(fileName).c_str());
		}

		float textWidth = textSize.x;
		// �e�L�X�g�̒����������s�����߂ɃJ�[�\���̈ʒu�𒲐�
		float posX = (buttonSize.x - textWidth) * 0.5f;
		ImGui::SetCursorPosX(posX + buttonPos.x);
		//ImGui::Text(fileName.c_str());
		if (!Utility::CharacterSearch(Utility::WStringToUTF8(fileName).c_str(), "cpp", { 0,1,1,1 }, Utility::WideStringToString(name)) &&
			!Utility::CharacterSearch(Utility::WStringToUTF8(fileName).c_str(), "h", { 1,0,1,1 }, Utility::WideStringToString(name)))
		{
			ImGui::Text(Utility::WStringToUTF8(fileName).c_str());
		}

		ImGui::EndGroup();
	}


	// �{�^���̉E�[���E�B���h�E�̕��𒴂���ꍇ�͉��s����
	if (buttonPos.x + buttonSize.x + buttonSpacing <= ImGui::GetWindowWidth() - 100)
	{
		ImGui::SameLine();
	}
}

void Fream_FileDialog_Item::FileAssignments(std::wstring& name, bool& buttonPressed, Vector2Flt buttonSize)
{
	if (Utility::IsHeaderFile(Utility::WStringToUTF8(name).c_str(), "cpp"))
	{
		ImGuiCustom::SetCustomButtonStyle(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGuiCustom::SetCustomButtonStyle(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 1));
		buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at("cpp"), ImVec2(buttonSize.x_, buttonSize.y_));
	}
	else if (Utility::IsHeaderFile(Utility::WStringToUTF8(name).c_str(), "h"))
	{
		// Windows�̋K��A�C�R�������[�h����
		HICON hIcon = LoadIcon(NULL, IDI_APPLICATION);

		ImGuiCustom::SetCustomButtonStyle(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 1));
		buttonPressed = ImGui::ImageButton((ImTextureID)hIcon, ImVec2(buttonSize.x_, buttonSize.y_));

		// �A�C�R�����������
		DestroyIcon(hIcon);
	}
	else
	{
		ImGuiCustom::SetCustomButtonStyle(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 1));
		buttonPressed = ImGui::ImageButton((void*)fileImageShaderDatas_.at("non"), ImVec2(buttonSize.x_,buttonSize.y_));
	}

	// �J�X�^���X�^�C�������ɖ߂�
	ImGui::StyleColorsClassic();

}

void Fream_FileDialog_Item::RenameWindow()
{
	auto path = fileFullPaht_ /= fileName_;

	static char buffer[256] = "";

	// ImGui�̃E�B���h�E���쐬
	ImGui::Begin(u8"��������");

	ImGui::Text(u8"�V�������O�̓���");
	// ImGui��InputText���g�p���ăe�L�X�g���͂�����
	ImGui::InputText("##�e�L�X�g����", buffer, sizeof(buffer));
	ImGui::SameLine();
	ImGuiCustom::HelpMarker(
	u8"�����܂�����{����͑Ή����ĂȂ��ł��A�Ȃ�ׂ������ł���悤�ɂ��܂�"
	);

	if (ImGui::Button(u8"�ύX�̊m��"))
	{
		// ���͕����񂪕ύX���ꂽ�ꍇ�̏������s��
		std::string newFileName = buffer;

		// �t�@�C�����̕ύX���������s����
		bool success = Utility::RenameFile(path.u8string(), newFileName);
		if (success)
		{
			// �t�@�C�����̕ύX�����������ꍇ�̏������s��
			MessageBox(NULL,"�t�@�C�����̕ύX�ɐ������܂����B","�t�@�C�����̕ύX",MB_OK);
			context_renameFlg_ = false;
		}
		else
		{
			// �t�@�C�����̕ύX�����s�����ꍇ�̏������s��
			MessageBox(NULL, "�t�@�C�����̕ύX�Ɏ��s���܂����B", "�t�@�C�����̕ύX", MB_OK);
			context_renameFlg_ = false;
		}
	}
	
	// ImGui�̃E�B���h�E�̏I��
	ImGui::End();
}

bool& Fream_FileDialog_Item::GetButton_Click()
{
	return button_click_;
}

void Fream_FileDialog_Item::Popup()
{
	if (ImGui::BeginPopup("ContextMenu"))
	{
		// �R���e�L�X�g���j���[�̓��e�������ɋL�q

		if (ImGui::MenuItem(u8"�؂���")) // ���j�[�N��ID���w�肷��
		{
			// �I�v�V����1���I�����ꂽ�Ƃ��̏���
			contextMenuFlg_ = false;
		}

		if (ImGui::MenuItem(u8"�R�s�[")) // ���j�[�N��ID���w�肷��
		{
			// �I�v�V����2���I�����ꂽ�Ƃ��̏���
			contextMenuFlg_ = false;
		}

		if (ImGui::MenuItem(u8"�\��t��")) // ���j�[�N��ID���w�肷��
		{
			// �I�v�V����2���I�����ꂽ�Ƃ��̏���
			contextMenuFlg_ = false;
		}

		if (ImGui::MenuItem(u8"���O�̕ύX")) // ���j�[�N��ID���w�肷��
		{
			context_renameFlg_ = true;
			// �I�v�V����2���I�����ꂽ�Ƃ��̏���
			contextMenuFlg_ = false;
		}
		ImGui::EndPopup();
	}
}
