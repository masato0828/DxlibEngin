#include "Fream_FileDialog.h"
#include "../../imGui/ImGuiFileDialog.h"
#include "../../imGui/CustomFont.h"
#include "../Common/FileDialog.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"
#include <codecvt>
#include <shlwapi.h>
#include <Windows.h>
#include <string>

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
}

void Fream_FileDialog::Update()
{
	// Imgui�p�E�B���h�E�N���X�̍쐬
	ImGuiWindowClass window_class;
	// �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
	
	
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
			
			if (nowSelect->parentFile_ != NULL)
			{
				if (IsMatch(directory.string(), StringToWideString(nowSelect->parentFile_->myName)))
				{
					// �c���[���J��
					ImGui::SetNextItemOpen(true);
				}
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
				Tree(directory, fileData_);
				ImGui::TreePop();
			}





			//	ImGui::TreePop();
			//}

		/*	ImGui::TreePop();
		}*/

			ImGui::End();
		}
		ImGui::End();
	}

	ImGui::SetNextWindowClass(&window_class);
	ItemWindow();

	
}

void Fream_FileDialog::Tree(std::filesystem::path directory, FileData& fileData)
{
	// �C�e���[�^�̍쐬
	std::filesystem::directory_iterator itr(directory.u8string());
	std::filesystem::directory_iterator end{};

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

		if (IsMatch(directoryName.c_str(), nowSelectFileName_))
		{
			// �c���[���J��
			ImGui::SetNextItemOpen(true);
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
			Tree(itr->path(), data->second);

			// �c���[�̏I��
			ImGui::TreePop();
		}
	}


}

bool Fream_FileDialog::CharacterSearch(std::string showName,std::string searchFileName,Color color, std::string defaultTarget)
{
	if(IsHeaderFile(defaultTarget,searchFileName))
	//if (defaultTarget.find(searchFileName) != std::string::npos)
	{
		ImGui::TextColored(ImVec4(color.r, color.g,color.b, color.a), showName.c_str());
		return true;
	}
	return false;
}

void Fream_FileDialog::ItemWindow()
{
	////// �����h�b�L���O�ݒ�
	//ImGuiID dockspace_id = ImGui::GetID("ShowFileNameLog");

	//if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
	//{
	//	ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
	//	ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node


	//	ImGuiID dock_main_id = dockspace_id;
	//	ImGuiID dock_id_FileNameLog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.001f, NULL, &dock_main_id);

	//	ImGui::DockBuilderDockWindow("FileNameLog", dock_main_id);

	//	ImGui::DockBuilderFinish(dockspace_id);
	//}

	//ImGuiID dockspace_id = ImGui::GetID("ShowFileNameLog");

	//if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
	//{
	//	ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
	//	ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

	//	ImGuiID dock_main_id = dockspace_id;
	//	ImGuiID dock_id_FileNameLog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.001f, NULL, &dock_main_id);

	//	ImGui::DockBuilderDockWindow("FileNameLog", dock_id_FileNameLog);

	//	ImGui::DockBuilderFinish(dockspace_id);
	//}

	if (ImGui::Begin("ITEM"))
	{
		//ImGui::DockSpace(dockspace_id, ImVec2(ImGui::GetWindowWidth(), 50.0f));

		ImGui::Separator();
		if (nowSelect != nullptr)
		{
			fileFullPaht_.clear();
			Recovery(nowSelect);
			fileFullPaht_ = fileFullPaht_.empty() ? std::filesystem::current_path() :
				std::filesystem::current_path() /= fileFullPaht_;
			ImGui::Separator();

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
					/*for (auto txt:nowSelectFileName_)
					{
						if (txt < -1)
						{
							nowSelectFileName_ = itrrrr.filename();
							break;
						}
					}*/

					fileFullPaht_ /= nowSelectFileName_;
					copySelectPath = nowSelectFileName_;

					// �t�H���_�̏ꍇ
					if (std::filesystem::is_directory(fileFullPaht_))
					{
						directoryName = fileFullPaht_.filename();
						
						nowSelect->fileMap_.try_emplace(WideStringToString(directoryName), FileData(nowSelect, WideStringToString(directoryName)));
						nowSelect = &nowSelect->fileMap_.at(WideStringToString(directoryName));
					}
					// �t�@�C���̏ꍇ
					else
					{
						OpenWithDefaultApplication(fileFullPaht_.wstring());
						/*if (IsHeaderFile(fileFullPaht_.string(), "cpp")||
							IsHeaderFile(fileFullPaht_.string(), "h")||
							IsHeaderFile(fileFullPaht_.string(), "txt"))
						{
							AppOpen();
						}*/
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
	ImGui::SetNextWindowSize(ImVec2(50, 10), ImGuiCond_Once);
	if (ImGui::Begin("FileNameLog"))
	{
		if (!std::filesystem::is_directory(nowSelectFile_))
		{
			// �E�B���h�E�̔w�i�F��ݒ�
			ImGui::Text(WideStringToString(nowSelectFile_).c_str());
		}
		ImGui::End();
	}
	ImGui::PopStyleColor();
}

void Fream_FileDialog::Recovery(FileData* selectData)
{
	if (selectData->parentFile_ != NULL)
	{
		Recovery(selectData->parentFile_);
		fileFullPaht_ += selectData->myName + "\\";
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

void Fream_FileDialog::MakeFileImage(std::wstring name)
{
	ImVec2 buttonPos = ImGui::GetCursorPos();
	ImVec2 buttonSize = ImVec2(90, 100);
	float buttonSpacing = 10.0f; // �{�^���Ԃ̃X�y�[�X

	ImGui::BeginGroup();
	{
		// ���j�[�N��ID���쐬
		ImGuiID buttonID = ImGui::GetID(name.c_str());

		// �{�^���𐶐�
		ImGui::PushID(buttonID);
		bool buttonPressed = ImGui::ImageButton((void*)my_shaderData, buttonSize);
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
		{
			// �{�^�����_�u���N���b�N���ꂽ�Ƃ��̏���
			nowSelectFileName_ = name.c_str();
		}
		ImGui::PopID();

		if (buttonPressed)
		{
			// �{�^���������ꂽ�Ƃ��̏���
			nowSelectFile_ = name.c_str();
		}

		// �e�L�X�g�̕����v�Z
		std::string fileName = WideStringToString(name);
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
		if (!CharacterSearch(fileName, "cpp", { 0,1,1,1 }, WideStringToString(name)) &&
			!CharacterSearch(fileName, "h", { 1,0,1,1 }, WideStringToString(name)))
		{
			ImGui::Text(fileName.c_str());
		}

		ImGui::EndGroup();
	}


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

		MakeFileImage(L"fileName");
	}

	
}

// std::string��wchar_t*�ɕϊ�����֐�
std::wstring Fream_FileDialog::ConvertToWideString(const std::string& str) 
{
	int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::vector<wchar_t> buffer(length);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.data(), length);
	return std::wstring(buffer.data());
}

std::wstring Fream_FileDialog::GetAssociatedApplicationPath(const std::wstring& extension)
{
	wchar_t progId[MAX_PATH];
	DWORD progIdSize = MAX_PATH;

	std::wstring messageContent;
	std::wstring errorMsg;
	std::wstring errorText;

	// �g���q�Ɋ֘A�t����ꂽ�v���O����ID���擾
	HRESULT result = AssocQueryStringW(ASSOCF_NONE, ASSOCSTR_EXECUTABLE, extension.c_str(), nullptr, progId, &progIdSize);
	if (result == S_OK)
	{
		// �v���O����ID������s�\�t�@�C���p�X���擾
		wchar_t executablePath[MAX_PATH];
		DWORD executablePathSize = MAX_PATH;

		result = AssocQueryStringW(ASSOCF_NONE, ASSOCSTR_EXECUTABLE, progId, nullptr, executablePath, &executablePathSize);
		if (result == S_OK)
		{
			return progId;
		}
		else
		{
			messageContent = L"progID�̎��s�\�p�X�̎擾�Ɏ��s���܂����B:";
			errorMsg = progId;
			errorText = messageContent + errorMsg;
			MessageBox(NULL, (LPCSTR)(errorText.c_str()), "error", MB_OK);
		}
	}
	else
	{
		messageContent = L"�g���q��ProgID�̎擾�Ɏ��s���܂����B:";
		errorMsg = extension.c_str();
		errorText = messageContent + errorMsg;
		MessageBox(NULL,(LPCSTR)(errorText.c_str()), "error", MB_OK);
	}

	MessageBox(NULL,"���̑��G���[","error",MB_OK);
	return L"";
}

std::wstring Fream_FileDialog::GetDefaultApplication(const std::wstring& extension)
{
	wchar_t progId[MAX_PATH];
	DWORD progIdSize = MAX_PATH;

	std::wstring messageContent;
	std::wstring errorMsg;
	std::wstring errorText;

	// �g���q�Ɋ֘A�t����ꂽ�v���O����ID���擾
	HRESULT result = AssocQueryStringW(ASSOCF_NONE, ASSOCSTR_EXECUTABLE, extension.c_str(), nullptr, progId, &progIdSize);
	if (result == S_OK)
	{
		// �v���O����ID������s�\�t�@�C���p�X���擾
		wchar_t executablePath[MAX_PATH];
		DWORD executablePathSize = MAX_PATH;

		result = AssocQueryStringW(ASSOCF_NONE, ASSOCSTR_EXECUTABLE, progId, nullptr, executablePath, &executablePathSize);
		if (result == S_OK)
		{
			return progId;
		}
		else
		{
			messageContent = L"progID�̎��s�\�p�X�̎擾�Ɏ��s���܂����B:";
			errorMsg = progId;
			errorText = messageContent + errorMsg;
			MessageBox(NULL, (LPCSTR)(errorText.c_str()), "error", MB_OK);
		}
	}
	else
	{
		messageContent = L"�g���q��ProgID�̎擾�Ɏ��s���܂����B:";
		MessageBox(NULL," �g���q��ProgID�̎擾�Ɏ��s���܂����B:", "error", MB_OK);
	}

	MessageBox(NULL, "���̑��G���[", "error", MB_OK);
	return L"";
}

bool Fream_FileDialog::LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath)
{
	std::wstring commandLine = L"\"";
	commandLine += applicationPath;
	commandLine += L"\" \"";
	commandLine += filePath;
	commandLine += L"\"";

	STARTUPINFOW startupInfo = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION processInfo;

	BOOL result = CreateProcessW(
		nullptr,
		&commandLine[0],
		nullptr,
		nullptr,
		FALSE,
		0,
		nullptr,
		nullptr,
		&startupInfo,
		&processInfo
	);

	if (result) {
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
		return true;
	}
	else {
		return false;
	}
}

bool Fream_FileDialog::OpenWithDefaultApplication(const std::wstring& filePath)
{
	std::wstring extension = filePath.substr(filePath.find_last_of(L"."));
	std::wstring defaultApplication = GetDefaultApplication(extension);

	if (!defaultApplication.empty()) {
		// �K��̃A�v���P�[�V�������ݒ肳��Ă���ꍇ�A������N������
		return LaunchApplication(defaultApplication, filePath);
	}
	else {
		// �K��̃A�v���P�[�V�������ݒ肳��Ă��Ȃ��ꍇ�A���[�U�[�ɃA�v���P�[�V������I������_�C�A���O��\������
		MessageBox(NULL,"text","title",MB_OK);

		// �_�C�A���O�̕\���R�[�h�������ɒǉ�����

		// �I�����ꂽ�A�v���P�[�V�����̃p�X���擾����
		std::wstring selectedApplication = L"�I�����ꂽ�A�v���P�[�V�����̃p�X";

		if (!selectedApplication.empty()) {
			// �I�����ꂽ�A�v���P�[�V�������N������
			return LaunchApplication(selectedApplication, filePath);
		}
		else {
			// �A�v���P�[�V�������I������Ȃ������ꍇ�̏���
			return false;
		}
	}
}

Fream_FileDialog::FileData::FileData(FileData* parent, std::string name)
{
	parentFile_ = parent;
	myName = name;
}

bool Fream_FileDialog::IsHeaderFile(const std::string& filename, const std::string& ext)
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

bool Fream_FileDialog::IsMatch(const std::string& filepath, const std::wstring& target) {
	std::filesystem::path pathObj(filepath);
	std::string filename = pathObj.filename().string();

	// �啶���Ə���������ʂ����ɔ�r����ꍇ
	/*if (filename == target) {
		return true;
	}*/

	// �啶���Ə���������ʂ��Ĕ�r����ꍇ
	// if (filename.compare(target) == 0) {
	//     return true;
	// }

	// �t�@�C�����Ɏw�肵�������񂪊܂܂�Ă��邩�𔻒肷��
	if (filename.find(WideStringToString(target)) != std::string::npos) {
		return true;
	}

	return false;
}

std::wstring Fream_FileDialog::StringToWideString(const std::string& str)
{
	std::wstring wideStr;
	wideStr.resize(str.size());

	std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(
		str.data(), str.data() + str.size(), wideStr.data()
	);

	return wideStr;
}

std::string Fream_FileDialog::WideStringToString(const std::wstring& wideString)
{
	return std::string(wideString.begin(), wideString.end());
}