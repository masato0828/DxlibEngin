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
	selected_ = 0;
	// 選択用マスク
	//selection_mask_ = (1 << 0);
}

void Fream_FileDialog::Update()
{
	if (ImGui::Begin("Project"))
	{
		// くっつけるスペースの作成のための変数
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("projectdock");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

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

			//// ツリーを選択
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			//if (ImGui::IsItemClicked())
			{
				// 選択している番号に変更
				node_clicked = cnt;
				selected_ = node_clicked;
			}

			// ノードが開かれている
			if (node_open)
			{
				// 名前変更
				Tree2(f, levelNo+1+ cnt);
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
		}
		else
		{
			//ImGui::Text(files.c_str());
		}

		cnt++;
	}
}
