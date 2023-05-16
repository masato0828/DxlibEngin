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
	// 選択している数
	nowSelect = nullptr;
	selected_ = 0;
	// 選択用マスク
	//selection_mask_ = (1 << 0);
	nowSelect = &fileData_;
	
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
					|ImGuiTreeNodeFlags_SpanAvailWidth;

				if (&fileData_ == nowSelect)
				{
					node_flags |= ImGuiTreeNodeFlags_Selected;
				}
					bool isopen = ImGui::TreeNodeEx((void*)&fileData_, node_flags, fileData_.myName.c_str());

					// ツリーを選択
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
					{
						// 現在選択中
						nowSelect = &fileData_;

					}
					if (isopen)
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
	std::filesystem::directory_iterator itr(directory.u8string());
	std::filesystem::directory_iterator end{};

	for (; itr != end; ++itr) 

	{	// ツリーデータの設定
		ImGuiTreeNodeFlags node_flags = 0;//=
			
			//ImGuiTreeNodeFlags_OpenOnDoubleClick |
			//ImGuiTreeNodeFlags_SpanAvailWidth;

		// ディレクトリがない場合
		if (!itr->is_directory())
		{
			// もう一度
			continue;
		}

		// イメージ
		// --------------------path-----------------------------
		// C: file1 // file2 // file3 // file4 // file5 // end{};
		// --------------------path-----------------------------の状態から
		// １ファイル分つまり//end{}を引かれ
		// C: file1 // file2 // file3 // file4 // file5となる
		// そこからitr->path().end()で一番最後のパスをとり
		// file5がとれる
		
		// この場合は(--itr->path().end()) でfile5が入る
		// ↓
		// フォルダ名の取得　（フルパスからの切り離し）
		auto directoryName = (--itr->path().end())->u8string();
		
		// データの作成　自分の
		//auto[dataItr,empFlag] = fileData.fileMap_.try_emplace(directoryName, FileData());
		auto data = fileData.fileMap_.try_emplace(directoryName.c_str(), FileData(&fileData, directoryName.c_str())).first;

		// 選択しているかどうか
		//const bool is_selected = &dataItr->second == nowSelect;
		const bool is_selected = &data->second == nowSelect;

		// 選択中
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		auto nowpath = itr->path();
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
		// ノードを開いているかどうか
		//auto is_open = ImGui::TreeNodeEx((void*)&dataItr->second, node_flags, directoryName.c_str());
		auto is_open = ImGui::TreeNodeEx((void*)&data->second, node_flags, directoryName.c_str());

		// ツリーを選択
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			// 現在選択中
			//nowSelect = &dataItr->second;
			nowSelect = &data->second;
		}

		// ノードが開かれている
		if (is_open)
		{
			// 再起処理
			//Tree2(itr->path(), dataItr->second);
			Tree2(itr->path(), data->second);
			// ツリーの終了
			ImGui::TreePop();
		}

		

	}
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

void Fream_FileDialog::ItemWindow()
{
	if (ImGui::Begin("ITEM"))
	{
		ImGui::Separator();
		auto directory = std::filesystem::current_path();
		std::string directry_str = directory.filename().u8string();
		//ImGui::Button();
		if (nowSelect != nullptr)
		{
			Recovery(nowSelect);
		}
		ImGui::Separator();


		/*ImGui::Button("button", ImVec2(100,100));
		ImGui::SameLine();
		ImGui::Button("button", ImVec2(100, 100));
		ImGui::SameLine();
		ImGui::Button("button", ImVec2(100, 100));
		ImGui::SameLine();
		ImGui::Button("button", ImVec2(100, 100));
		ImGui::SameLine();
		ImGui::Button("button", ImVec2(100, 100));
		ImGui::SameLine();
		ImGui::Button("button", ImVec2(100, 100));
		ImGui::SameLine();
		ImGui::Button("button", ImVec2(100, 100));
		ImGui::Text("text1");
		ImGui::SameLine();
		ImGui::Text("text2");
		ImGui::SameLine();
		ImGui::Text("text3");
		ImGui::SameLine();
		ImGui::Text("text4");
		ImGui::SameLine();
		ImGui::Text("text5");
		ImGui::SameLine();
		ImGui::Text("text6");
		ImGui::SameLine();
		ImGui::Text("text7");
		ImGui::SameLine();*/


		CreateImage();
		MakeFileImage(u8"text");
		ImGui::SameLine();

		MakeFileImage(u8"text1");
		ImGui::SameLine();

		MakeFileImage(u8"text2");
		ImGui::SameLine();
		ImGui::End();
	}
}

void Fream_FileDialog::Recovery(FileData* selectData)
{
	if (selectData->parentFile_ != NULL)
	{
		Recovery(selectData->parentFile_);
		ImGui::SameLine();
		ImGui::Button("/");
	}
	ImGui::SameLine();
	//	ImGui::Button(selectData->myName.c_str());
	if (ImGui::Button(selectData->myName.c_str()))
	{
		nowSelect = selectData;
		return;
	}
}

void Fream_FileDialog::MakeFileImage( std::string_view name)
{
	ImGui::BeginGroup();

	ImVec2 buttonPos = ImGui::GetCursorPos();
	ImVec2 buttonSize = ImVec2(100, 100);
	ImGui::ImageButton((void*)my_shaderData, buttonSize);
	// テキストの幅を計算
	{
		ImVec2 textSize = ImGui::CalcTextSize(name.data());
		float textWidth = textSize.x;

		// テキストの中央揃えを行うためにカーソルの位置を調整
		float posX = (buttonSize.x - textWidth) * 0.5f;
		ImGui::SetCursorPosX(posX+ buttonPos.x);
		ImGui::Text(name.data());
	}
	ImGui::EndGroup();
}

void Fream_FileDialog::CreateImage()
{
	
	// 画像の読み込み
	auto defaultImageSize_ = Vector2();
	bool ret = LoadBackGroundTextureFromFile(&my_shaderData, &defaultImageSize_.x_, &defaultImageSize_.y_);

	auto sizeX = ImGui::GetWindowSize().x;
	auto sizeY = ImGui::GetWindowSize().y;

	// 横割合
	auto x = ImGui::GetWindowSize().x / defaultImageSize_.x_;
	// 縦割合
	auto y = ImGui::GetWindowSize().y / defaultImageSize_.y_;


	// 係数
	auto factor = (std::min)(x, y);

	if (factor == 0)
	{
		ImGui::End();
		return;
	}

	//ImGui::Image(
	//	// 画像情報
	//	(void*)my_shaderData,
	//	// 画像サイズを割合で変える
	//	ImVec2{ defaultImageSize_.x_ * factor, defaultImageSize_.y_ * factor },
	//	// UV1
	//	ImVec2{ 0,0 },
	//	// UV2
	//	ImVec2{ 1,1 },
	//	// 描画カラー
	//	ImVec4{ 1,1,1,1 },
	//	// 枠のカラー
	//	ImVec4{ 0,0,0,0 });
}

Fream_FileDialog::FileData::FileData(FileData* parent, std::string name)
{
	parentFile_ = parent;
	myName = name;
}
