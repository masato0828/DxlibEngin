#include "Fream_FileDialog.h"
#include "../../imGui/ImGuiFileDialog.h"
#include "../../imGui/CustomFont.h"
#include "../Common/FileDialog.h"
#include "../../imGui/imgui_internal.h"

Fream_FileDialog::Fream_FileDialog()
{
	Init();
}

Fream_FileDialog::~Fream_FileDialog()
{
}

void Fream_FileDialog::Init()
{
	// �I�����Ă��鐔
	selected_ = 0;
	// �I��p�}�X�N
	//selection_mask_ = (1 << 0);
}

void Fream_FileDialog::Update()
{
	if (ImGui::Begin("Project"))
	{
		// ��������X�y�[�X�̍쐬�̂��߂̕ϐ�
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("projectdock");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		// Imgui�p�E�B���h�E�N���X�̍쐬
		ImGuiWindowClass window_class;
		// �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);
		if (ImGui::Begin("fileDialog",0))
		{
			//���݂̃J�������g�f�B���N�g�����擾
			auto directory = std::filesystem::current_path();
			// �v���O�����N�����Ƀt�@�C���̏o��
			// ���̈ʒu����c���[���J��

			if (ImGui::TreeNode("file"))
			{
				int eraseNum = directory.string().find_last_of('\\');
				//// 
				std::string file = directory.string().erase(0, eraseNum);
				//// 
				std::string fileN = file.erase(0, 1);

				if (ImGui::TreeNode(fileN.c_str()))
				{
					selection_mask_vector_.push_back((1 << -2));
					Tree2(directory,0);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			ImGui::End();
		}

		ImGui::SetNextWindowClass(&window_class);
		if (ImGui::Begin("ITEM"))
		{
			ImGui::End();
		}

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
			//ImGui::Text(files.c_str());
		}

		cnt++;
	}
}

void Fream_FileDialog::Tree2(std::filesystem::path directory, int levelNo)
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

			//// �c���[��I��
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			//if (ImGui::IsItemClicked())
			{
				// �I�����Ă���ԍ��ɕύX
				node_clicked = cnt;
				selected_ = node_clicked;
			}

			// �m�[�h���J����Ă���
			if (node_open)
			{
				// ���O�ύX
				Tree2(f, levelNo+1+ cnt);
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
		}
		else
		{
			//ImGui::Text(files.c_str());
		}

		cnt++;
	}
}
