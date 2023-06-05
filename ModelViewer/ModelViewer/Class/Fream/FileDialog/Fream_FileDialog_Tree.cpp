#include "Fream_FileDialog_Tree.h"
#include "../../../imGui/imgui.h"
#include "../../../imGui/ImGuiFileDialog.h"
#include "../../../imGui/CustomFont.h"
#include "../../Common/FileDialog.h"
#include "../../../imGui/imgui_internal.h"
#include "../../Common/ImGuiMyCustom.h"
#include "../../Common/Utility.h"


Fream_FileDialog_Tree::Fream_FileDialog_Tree()
{
}

Fream_FileDialog_Tree::~Fream_FileDialog_Tree()
{
}

void Fream_FileDialog_Tree::Init()
{
}

void Fream_FileDialog_Tree::Update()
{
}

void Fream_FileDialog_Tree::Update(
	FileData& fileData,
	FileData*& nowSelect,
	std::filesystem::path& nowSelectPath,
	std::wstring& nowSelectFileName)
{
	nowSelectPath_ = nowSelectPath;
	nowSelectFileName_ = nowSelectFileName;

	if (ImGui::Begin("FileTree"))
	{
		//���݂̃J�������g�f�B���N�g�����擾
		auto directory = std::filesystem::current_path();

		ImGuiTreeNodeFlags node_flags =
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		if (&fileData == nowSelect)
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (nowSelect->parentFile_ != NULL)
		{
			if (IsMatch(directory.string(), Utility::StringToWideString(nowSelect->parentFile_->myName)))
			{
				// �c���[���J��
				ImGui::SetNextItemOpen(true);
			}
		}

		bool is_open = ImGui::TreeNodeEx((void*)&fileData, node_flags, fileData.myName.c_str());

		// �c���[��I��
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			// ���ݑI��
			nowSelect = &fileData;
		}
		if (is_open)
		{
			Tree(directory, fileData,nowSelect);
			ImGui::TreePop();
		}

		ImGui::End();
	}

	nowSelectPath = nowSelectPath_;
	nowSelectFileName = nowSelectFileName_;
}

void Fream_FileDialog_Tree::Tree(std::filesystem::path directory, FileData& fileData, FileData*& nowSelect)
{
	// �C�e���[�^�̍쐬
	std::filesystem::directory_iterator itr(Utility::WideStringToString((Utility::StringToWideString(directory.string()))));
	std::filesystem::directory_iterator end{};

	for (; itr != end; ++itr)
	{
		// �c���[�f�[�^�̐ݒ�
		ImGuiTreeNodeFlags node_flags = 0;

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
			Tree(itr->path(), data->second,nowSelect);

			// �c���[�̏I��
			ImGui::TreePop();
		}
	}


}

bool Fream_FileDialog_Tree::IsMatch(const std::string& filepath, const std::wstring& target)
{
	std::filesystem::path pathObj(filepath);
	std::string filename = pathObj.filename().string();

	if (target.empty())
	{
		return false;
	}

	// �啶���Ə���������ʂ����ɔ�r����ꍇ
	/*if (filename == target) {
		return true;
	}*/

	// �啶���Ə���������ʂ��Ĕ�r����ꍇ
	// if (filename.compare(target) == 0) {
	//     return true;
	// }

	// �t�@�C�����Ɏw�肵�������񂪊܂܂�Ă��邩�𔻒肷��
	if (filename.find(Utility::WideStringToString(target)) != std::string::npos) {
		return true;
	}

	return false;
}