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
	// 選択している数
	nowSelect = nullptr;
	selected_ = 0;
	// 選択用マスク
	//selection_mask_ = (1 << 0);
}

void Fream_FileDialog::Update()
{
	if (ImGui::Begin("Project"))
	{
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

		// Imgui用ウィンドウクラスの作成
		ImGuiWindowClass window_class;
		// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);
		if (ImGui::Begin("fileDialog",0))
		{
			//現在のカンレントディレクトリを取得
			auto directory = std::filesystem::current_path();
			// プログラム起動時にファイルの出力
			// その位置からツリーを開く

			if (ImGui::TreeNode("file"))
			{
				int eraseNum = directory.string().find_last_of('\\');
				//// 
				std::string file = directory.string().erase(0, eraseNum);
				//// 
				std::string fileN = file.erase(0, 1);

				if (ImGui::TreeNode(fileN.c_str()))
				{
					selection_mask_vector_.push_back((0 << 1));
					//Tree(directory,0);
					Tree2(directory,fileData_);
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			ImGui::End();
		}

		ImGui::SetNextWindowClass(&window_class);
		if (ImGui::Begin("ITEM"))
		{
			ImGui::Text("ITEM");
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
	// 現在の選択番号
	int node_clicked = -1;

	

	for (auto f : e)
	{
		
		// ツリーデータの設定
		ImGuiTreeNodeFlags node_flags =
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		// 選択しているかどうか
		const bool is_selected = (selection_mask_vector_.at(levelNo) & (1 << cnt)) != 0;
		// 選択中
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		// \\がある所までの文字数を取得
		int eraseNums = f.path().u8string().find_last_of('\\');
		// 絶対パスからeraseNumsまでを削除
		std::string files = f.path().u8string().erase(0, eraseNums);
		// filesから\\を削除
		std::string fileNs = files.erase(0, 1);
		// ファイルかフォルダかチェック
		bool isDirectory = std::filesystem::is_directory(f.path());

		if (isDirectory)
		{
			// ノードを開いているかどうか
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)cnt, node_flags, files.c_str());

			// ツリーを選択
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				// 選択している番号に変更
				node_clicked = cnt;
				selected_ = node_clicked;
				selection_mask_vector_.at(levelNo+cnt) = (1 << 0);
			}

			// ノードが開かれている
			if (node_open)
			{
				selection_mask_vector_.push_back((1 << 0));
				// 名前変更
				Tree(f,levelNo+1+cnt);
				// ツリーの終了
				ImGui::TreePop();

			}

			// 現在ツリーをクリックしていない
			if (node_clicked != -1)
			{
				if (ImGui::GetIO().KeyCtrl)
					selection_mask_vector_.at(levelNo) ^= (1 << node_clicked);          // CTRL+click to toggle
				else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
					selection_mask_vector_.at(levelNo) = (1 << node_clicked);           // Click to single-select
			}

			
			//// 名前とする
			//if (ImGui::TreeNode(files.c_str()))
			//{
			//	// 再帰
			//	Tree(f);
			//	// ツリーの終了処理
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
	// イテレータの作成
	std::filesystem::directory_iterator itr(directory.string());
	std::filesystem::directory_iterator end{};




	for (; itr != end; ++itr) 

	{	// ツリーデータの設定
		ImGuiTreeNodeFlags node_flags =
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		if (!itr->is_directory())
		{
			continue;
		}

		auto directoryName = (--itr->path().end())->u8string();
		auto[dataItr,empFlag] = fileData.fileMap_.try_emplace(directoryName, FileData());

		

		// 選択しているかどうか
		const bool is_selected = &dataItr->second == nowSelect;
		// 選択中
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;
		// ノードを開いているかどうか

		auto test = ImGui::TreeNodeEx((void*)&dataItr->second, node_flags, directoryName.c_str());

		// ツリーを選択
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			nowSelect = &dataItr->second;
		}

		if (test)
		{
			Tree2(itr->path(), dataItr->second);
			ImGui::TreePop();
		}
		
	}
	// データの作成
	//fileData_.fileMap_.emplace("名前", FileData());
}

bool Fream_FileDialog::CharacterSearch(std::string target,std::string searchFileName,Color color)
{
	if (target.find(searchFileName) != std::string::npos)
	{
		ImGui::TextColored(ImVec4(color.r, color.g,color.b, color.a), target.c_str());
		return true;
	}
	return false;
}
