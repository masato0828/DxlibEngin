#include "Fream_FileDialog.h"
#include "../../imGui/ImGuiFileDialog.h"
#include "../../imGui/CustomFont.h"
#include "../Common/FileDialog.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"


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

	nowSelectFileName_ = "";
}

void Fream_FileDialog::Update()
{
	if (ImGui::Begin("Project"))
	{
		// �����h�b�L���O�ݒ�
		ImGuiID dockspace_id = ImGui::GetID("projectDock");
		if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
		{
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

			ImGuiID dock_main_id = dockspace_id;
			ImGuiID dock_id_FileDialog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 1.f, NULL, &dock_main_id);

			ImGui::DockBuilderSetNodeSize(dock_main_id, ImVec2(300, 200));
			ImGui::DockBuilderSetNodeSize(dock_id_FileDialog, ImVec2(293, 200));

			ImGui::DockBuilderDockWindow("fileDialog", dock_id_FileDialog);
			ImGui::DockBuilderDockWindow("ITEM", dock_main_id);

			ImGui::DockBuilderFinish(dockspace_id);
		}
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		mainWindowSize_ = { ImGui::GetWindowSize().x,ImGui::GetWindowSize().y};

		// Imgui�p�E�B���h�E�N���X�̍쐬
		ImGuiWindowClass window_class;
		// �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);
		if (ImGui::Begin("fileDialog", 0))
		{
			//���݂̃J�������g�f�B���N�g�����擾
			auto directory = std::filesystem::current_path();
			// �v���O�����N�����Ƀt�@�C���̏o��
			// ���̈ʒu����c���[���J��

			//if (ImGui::TreeNode("file"))
			//{
			int eraseNum = directory.string().find_last_of('\\');
			//// 
			std::string file = directory.string().erase(0, eraseNum);
			//// 
			std::string fileN = file.erase(0, 1);

			//if (ImGui::TreeNode(fileN.c_str()))
			//{
			//	selection_mask_vector_.push_back((0 << 1));
				//Tree(directory,0);
			ImGuiTreeNodeFlags node_flags =
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_OpenOnDoubleClick
				| ImGuiTreeNodeFlags_SpanAvailWidth;

			if (&fileData_ == nowSelect)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}

			//if (fileData_.myName)
			//nowSelect
			if(directory == nowSelect->myName)
			{
				node_flags |= ImGuiTreeNodeFlags_DefaultOpen;
			}

			bool is_open = ImGui::TreeNodeEx((void*)&fileData_, node_flags, fileData_.myName.c_str());

			// �c���[��I��
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				// ���ݑI��
				nowSelect = &fileData_;

			}
			if (is_open)
			{
				Tree2(directory, fileData_);
				ImGui::TreePop();
			}





			//	ImGui::TreePop();
			//}

		/*	ImGui::TreePop();
		}*/

			ImGui::End();
		}

		ImGui::SetNextWindowClass(&window_class);
		ItemWindow();
		
		ImGui::End();
	}
}

void Fream_FileDialog::Tree(std::filesystem::path  directory,int levelNo)
{
	//auto fii = ".\\" + fi;
	std::filesystem::directory_iterator e = std::filesystem::directory_iterator(directory.string());

	int cnt = 0;
	// ���݂̑I��ԍ�
	int node_clicked = -1;

	

	for (auto f : e)
	{
		
		// �c���[�f�[�^�̐ݒ�
		ImGuiTreeNodeFlags node_flags =
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		// �I�����Ă��邩�ǂ���
		const bool is_selected = (selection_mask_vector_.at(levelNo) & (1 << cnt)) != 0;
		// �I��
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		// \\�����鏊�܂ł̕��������擾
		int eraseNums = f.path().u8string().find_last_of('\\');
		// ��΃p�X����eraseNums�܂ł��폜
		std::string files = f.path().u8string().erase(0, eraseNums);
		// files����\\���폜
		std::string fileNs = files.erase(0, 1);
		// �t�@�C�����t�H���_���`�F�b�N
		bool isDirectory = std::filesystem::is_directory(f.path());

		if (isDirectory)
		{
			// �m�[�h���J���Ă��邩�ǂ���
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)cnt, node_flags, files.c_str());

			// �c���[��I��
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				// �I�����Ă���ԍ��ɕύX
				node_clicked = cnt;
				selected_ = node_clicked;
				selection_mask_vector_.at(levelNo+cnt) = (1 << 0);
			}

			// �m�[�h���J����Ă���
			if (node_open)
			{
				selection_mask_vector_.push_back((1 << 0));
				// ���O�ύX
				Tree(f,levelNo+1+cnt);
				// �c���[�̏I��
				ImGui::TreePop();

			}

			// ���݃c���[���N���b�N���Ă��Ȃ�
			if (node_clicked != -1)
			{
				if (ImGui::GetIO().KeyCtrl)
					selection_mask_vector_.at(levelNo) ^= (1 << node_clicked);          // CTRL+click to toggle
				else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
					selection_mask_vector_.at(levelNo) = (1 << node_clicked);           // Click to single-select
			}

			
			//// ���O�Ƃ���
			//if (ImGui::TreeNode(files.c_str()))
			//{
			//	// �ċA
			//	Tree(f);
			//	// �c���[�̏I������
			//	ImGui::TreePop();
			//}
		}
		else
		{
			////ImGui::Text(files.c_str());
			//if (!CharacterSearch(files, ".cpp", { 1,1,0,1 }) &&
			//	!CharacterSearch(files, ".h", { 0,1,0,1 }) &&
			//	!CharacterSearch(files, ".txt", { 1,1,1,1 }))
			//{
			//	ImGui::Text(files.c_str());
			//}
		}

		cnt++;
	}
}

void Fream_FileDialog::Tree2(std::filesystem::path directory, FileData& fileData)
{
	// �C�e���[�^�̍쐬
	std::filesystem::directory_iterator itr(directory.u8string());
	std::filesystem::directory_iterator end{};
	directory;
	nowSelect;
	for (; itr != end; ++itr) 
	{	
		// �c���[�f�[�^�̐ݒ�
		ImGuiTreeNodeFlags node_flags = 0;//=
			
			//ImGuiTreeNodeFlags_OpenOnDoubleClick |
			//ImGuiTreeNodeFlags_SpanAvailWidth;

		// �f�B���N�g�����Ȃ��ꍇ
		if (!itr->is_directory())
		{
			// ������x
			continue;
		}

		if (fileData.myName == nowSelect->myName)
		{
			node_flags |= ImGuiTreeNodeFlags_DefaultOpen;
		}

		// �C���[�W
		// --------------------path-----------------------------
		// C: file1 // file2 // file3 // file4 // file5 // end{};
		// --------------------path-----------------------------�̏�Ԃ���
		// �P�t�@�C�����܂�//end{}��������
		// C: file1 // file2 // file3 // file4 // file5�ƂȂ�
		// ��������itr->path().end()�ň�ԍŌ�̃p�X���Ƃ�
		// file5���Ƃ��
		
		// ���̏ꍇ��(--itr->path().end()) ��file5������
		// ��
		// �t�H���_���̎擾�@�i�t���p�X����̐؂藣���j
		auto directoryName = (--itr->path().end())->u8string();
		
		// �f�[�^�̍쐬�@������
		//auto[dataItr,empFlag] = fileData.fileMap_.try_emplace(directoryName, FileData());
		auto data = fileData.fileMap_.try_emplace(directoryName.c_str(), FileData(&fileData, directoryName.c_str())).first;

		// �I�����Ă��邩�ǂ���
		//const bool is_selected = &dataItr->second == nowSelect;
		const bool is_selected = &data->second == nowSelect;

		// �I��
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		std::filesystem::recursive_directory_iterator subItr(itr->path());

		for (auto& sub : subItr)
		{
			if (sub.is_directory())
			{ 
				node_flags = node_flags & ~ImGuiTreeNodeFlags_Leaf;
				break;
			} 
			node_flags |= ImGuiTreeNodeFlags_Leaf;
		}
		// �m�[�h���J���Ă��邩�ǂ���
		//auto is_open = ImGui::TreeNodeEx((void*)&dataItr->second, node_flags, directoryName.c_str());
		auto is_open = ImGui::TreeNodeEx((void*)&data->second, node_flags, directoryName.c_str());

		// �c���[��I��
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			// ���ݑI��
			//nowSelect = &dataItr->second;
			nowSelect = &data->second;
			nowSelectPath_ = itr->path();
		}

		// �m�[�h���J����Ă���
		if (is_open)
		{
			// �ċN����
			//Tree2(itr->path(), dataItr->second);
			Tree2(itr->path(), data->second);

			// �c���[�̏I��
			ImGui::TreePop();
		}

		

	}
}

bool Fream_FileDialog::CharacterSearch(std::string showName,std::string searchFileName,Color color, std::string defaultTarget)
{
	if(isHeaderFile(defaultTarget,searchFileName))
	//if (defaultTarget.find(searchFileName) != std::string::npos)
	{
		ImGui::TextColored(ImVec4(color.r, color.g,color.b, color.a), showName.c_str());
		return true;
	}
	return false;
}

void Fream_FileDialog::ItemWindow()
{
	//// �����h�b�L���O�ݒ�
	ImGuiID dockspace_id = ImGui::GetID("ShowFileNameLog");

	if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
	{
		ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_id_FileNameLog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.1f, NULL, &dock_main_id);

		ImGui::DockBuilderSetNodeSize(dock_main_id, ImVec2(293, 1));

		ImGui::DockBuilderDockWindow("FileNameLog", dock_main_id);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	if (ImGui::Begin("ITEM"))
	{
		
		ImGui::DockSpace(dockspace_id, ImVec2(ImGui::GetWindowWidth(), 50.0f));

		ImGui::Separator();
		if (nowSelect != nullptr)
		{
			fileFullPaht_.clear();
			Recovery(nowSelect);
			fileFullPaht_ = fileFullPaht_.size() == 0 ? std::filesystem::current_path().string() :
				std::filesystem::current_path().string() + "/" + fileFullPaht_;
			ImGui::Separator();

			auto current_path = std::filesystem::current_path();

			// �C�e���[�^�̍쐬
			std::filesystem::directory_iterator itr(fileFullPaht_);
			std::filesystem::directory_iterator end{};
			std::string directoryName;

			auto copySelectPath = nowSelectFileName_;
			for (; itr != end; ++itr)
			{
				directoryName = (--itr->path().end())->u8string();

				MakeFileImage(directoryName/*���݃t�H���_�̉��̊K�w�̃t�@�C���y�уt�H���_�̖��O*/);

				if (nowSelectFileName_ != copySelectPath)
				{
					fileFullPaht_ += ("/" + nowSelectFileName_);
					copySelectPath = nowSelectFileName_;

					// �t�H���_�̏ꍇ
					if (std::filesystem::is_directory(fileFullPaht_))
					{
						std::filesystem::path path(fileFullPaht_);
						auto directoryName = (--path.end())->u8string();
						nowSelect->fileMap_.try_emplace(directoryName.c_str(), FileData(nowSelect, directoryName.c_str()));
						nowSelect = &nowSelect->fileMap_.at(directoryName.c_str());
					}
					// �t�@�C���̏ꍇ
					else
					{
						if (isHeaderFile(fileFullPaht_, "cpp"))
						{
							//AppOpen();
							// edit file open
						}
					}
				}
			}

			

		}

		ImGui::End();
	}

	// Imgui�p�E�B���h�E�N���X�̍쐬
	ImGuiWindowClass window_class;
	// �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiCond_Once);
	ImGui::Begin("FileNameLog");
	// �E�B���h�E�̔w�i�F��ݒ�
	ImGui::Text(nowSelectFileName_.c_str());
	ImGui::End();
	ImGui::PopStyleColor();
}

void Fream_FileDialog::Recovery(FileData* selectData)
{
	if (selectData->parentFile_ != NULL)
	{
		Recovery(selectData->parentFile_);
		fileFullPaht_ += selectData->myName + "/";
		ImGui::SameLine();
		ImGui::Button("/");
	}
	ImGui::SameLine();
	if (ImGui::Button(selectData->myName.c_str()))
	{
		nowSelect = selectData;
		//fileFullPaht_ += selectData->myName + "/";
		return;
	}
}

void Fream_FileDialog::MakeFileImage( std::string name)
{
	ImVec2 buttonPos = ImGui::GetCursorPos();
	ImVec2 buttonSize = ImVec2(90, 100);
	float buttonSpacing = 10.0f; // �{�^���Ԃ̃X�y�[�X

	ImGui::BeginGroup();

	// ���j�[�N��ID���쐬
	ImGuiID buttonID = ImGui::GetID(name.c_str());

	// �{�^���𐶐�
	ImGui::PushID(buttonID);
	bool buttonPressed = ImGui::ImageButton((void*)my_shaderData, buttonSize);
	ImGui::PopID();

	if (buttonPressed)
	{
		// �{�^���������ꂽ�Ƃ��̏���
		nowSelectFileName_ = name;
	}

	// �e�L�X�g�̕����v�Z
	std::string fileName = name;
	ImVec2 textSize = ImGui::CalcTextSize(fileName.c_str());

	// �{�^���̕��ƕ�����̒������r���A�{�^���̕��𒴂���ꍇ�͕������؂�l�߂� "..." ��ǉ�
	if (textSize.x > buttonSize.x)
	{
		while (textSize.x > buttonSize.x && fileName.length() > 0)
		{
			fileName.pop_back();
			// �e�L�X�g�̃T�C�Y�𖈉�X�V����
			textSize = ImGui::CalcTextSize(fileName.c_str());
		}
		fileName += "...";
		// ������̃T�C�Y���擾
		textSize = ImGui::CalcTextSize(fileName.c_str());
	}

	float textWidth = textSize.x;
	// �e�L�X�g�̒����������s�����߂ɃJ�[�\���̈ʒu�𒲐�
	float posX = (buttonSize.x - textWidth) * 0.5f;
	ImGui::SetCursorPosX(posX + buttonPos.x);
	//ImGui::Text(fileName.c_str());
	if (!CharacterSearch(fileName, "cpp", { 0,1,1,1 }, name)&&
		!CharacterSearch(fileName, "h", { 1,0,1,1 }, name))
	{
		ImGui::Text(fileName.c_str());
	}
	ImGui::EndGroup();

	// �{�^���̉E�[���E�B���h�E�̕��𒴂���ꍇ�͉��s����
	if (buttonPos.x + buttonSize.x + buttonSpacing <= ImGui::GetWindowWidth() - 100)
	{
		ImGui::SameLine();
	}
}

void Fream_FileDialog::CreateImage(std::string filePath)
{
	
	// �摜�̓ǂݍ���
	auto defaultImageSize_ = Vector2();
	//bool ret = LoadBackGroundTextureFromFile(&my_shaderData, &defaultImageSize_.x_, &defaultImageSize_.y_);
	bool ret = false; 
	
	if (my_shaderData_vector_.size() != NULL)
	{
		ret = LoadTextureFromFile(
			filePath.c_str(),// �摜�t�@�C���̏ꏊ
			&my_shaderData_vector_.at(my_shaderData_vector_.size() - 1),	// �V�F�[�_�̃f�[�^�쐬�p�̕ϐ�
			&defaultImageSize_.x_,// �摜�̃T�C�Y�擾�ϐ�
			&defaultImageSize_.y_);

		if (!ret)
		{
			my_shaderData_vector_.at(my_shaderData_vector_.size() - 1) = NULL;
		}

		MakeFileImage("fileName");
	}

	
}

void Fream_FileDialog::AppOpen()
{
	// �N���������A�v���P�[�V�����̃t���p�X(���邢�̓J�����g�t�H���_�[����̑��΃p�X)�ƁA
   // �K�v�ɉ����Ĉ������󔒋�؂�ɂĕt������B
	wchar_t commandLine[] = L"C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe";

	STARTUPINFO si{};
	PROCESS_INFORMATION pi{};

	si.cb = sizeof(si);

	// �R�}���h���C���̎��s(���������0�ȊO���߂�)
	if (CreateProcess(nullptr, (LPSTR)commandLine, nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi))
	{
		// �N������
		//

		// �A�v���P�[�V�����̏I���܂ő҂�
		// (�҂����ɏ������ɐi�߂�ꍇ�AWaitForSingleObject��GetExitCodeProcess�͕s�v)
		WaitForSingleObject(pi.hProcess, INFINITE);

		// �A�v���P�[�V�����̏I���R�[�h�̎擾
		// (�I���R�[�h���g�p���Ȃ��ꍇ�͕s�v)
		unsigned long exitCode;
		GetExitCodeProcess(pi.hProcess, &exitCode);

		// �I���R�[�h�����̒l�ɂȂ�ꍇ������̂ŁAsigned�ɃL���X�g����
		long ec = static_cast<long>(exitCode);

		// �n���h�������
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		// �N�����s
		//
	}
}

Fream_FileDialog::FileData::FileData(FileData* parent, std::string name)
{
	parentFile_ = parent;
	myName = name;
}

bool Fream_FileDialog::isHeaderFile(const std::string& filename, const std::string& ext)
{	
	// �g���q�̈ʒu��T��
	std::size_t dotPos = filename.find_last_of(".");
	if (dotPos == std::string::npos) {
		// �h�b�g��������Ȃ��ꍇ�͊g���q���Ȃ��Ɣ��f
		return false;
	}

	// �g���q���擾���Ĕ�r����
	std::string extension = filename.substr(dotPos + 1);
	if (extension == ext) {
		return true;
	}

	return false;
}

void Fream_FileDialog::ChangeNowSelect(FileData* selectData)
{

}