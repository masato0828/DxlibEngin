#include "Fream_FileDialog.h"
#include "../../imGui/ImGuiFileDialog.h"
#include "../../imGui/CustomFont.h"
#include "../Common/FileDialog.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"
#include <codecvt>

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

	nowSelectFileName_ = "";
}

void Fream_FileDialog::Update()
{
	if (ImGui::Begin("Project"))
	{
		// 強制ドッキング設定
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

		// Imgui用ウィンドウクラスの作成
		ImGuiWindowClass window_class;
		// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);
		if (ImGui::Begin("fileDialog", 0))
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
				| ImGuiTreeNodeFlags_SpanAvailWidth;

			if (&fileData_ == nowSelect)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}
			
			if (nowSelect->parentFile_ != NULL)
			{
				if (isMatch(directory.string(), nowSelect->parentFile_->myName))
				{
					// ツリーを開く
					ImGui::SetNextItemOpen(true);
				}
			}


			bool is_open = ImGui::TreeNodeEx((void*)&fileData_, node_flags, fileData_.myName.c_str());

			

			// ツリーを選択
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				// 現在選択中
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
	{	
		// ツリーデータの設定
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

		if (isMatch(directoryName.c_str(), nowSelectFileName_))
		{
			// ツリーを開く
			ImGui::SetNextItemOpen(true);
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
			nowSelectPath_ = itr->path();
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
	//// 強制ドッキング設定
	ImGuiID dockspace_id = ImGui::GetID("ShowFileNameLog");

	if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
	{
		ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
		

		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_id_FileNameLog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.001f, NULL, &dock_main_id);

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

			// イテレータの作成
			std::filesystem::directory_iterator itr(fileFullPaht_);
			std::filesystem::directory_iterator end{};
			std::string directoryName;

			auto copySelectPath = nowSelectFileName_;
			for (; itr != end; ++itr)
			{
				directoryName = (--itr->path().end())->u8string();

				MakeFileImage(directoryName/*現在フォルダの下の階層のファイル及びフォルダの名前*/);

				if (nowSelectFileName_ != copySelectPath)
				{
					fileFullPaht_ += ("/" + nowSelectFileName_);
					copySelectPath = nowSelectFileName_;

					// フォルダの場合
					if (std::filesystem::is_directory(stringToWideString(fileFullPaht_)))
					{
						std::filesystem::path path(stringToWideString(fileFullPaht_));
						auto directoryName = (--path.end())->u8string();
						nowSelect->fileMap_.try_emplace(directoryName.c_str(), FileData(nowSelect, directoryName.c_str()));
						nowSelect = &nowSelect->fileMap_.at(directoryName.c_str());



					}
					// ファイルの場合
					else
					{
						if (isHeaderFile(fileFullPaht_, "cpp"))
						{
							AppOpen();
						}
					}
				}
			}

			

		}

		ImGui::End();
	}

	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_class;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	ImGui::SetNextWindowSize(ImVec2(50, 10), ImGuiCond_Once);
	if (ImGui::Begin("FileNameLog"))
	{
		if (!std::filesystem::is_directory(stringToWideString(nowSelectFileName_)))
		{
			// ウィンドウの背景色を設定
			ImGui::Text(nowSelectFileName_.c_str());
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

void Fream_FileDialog::MakeFileImage(std::string name)
{
	ImVec2 buttonPos = ImGui::GetCursorPos();
	ImVec2 buttonSize = ImVec2(90, 100);
	float buttonSpacing = 10.0f; // ボタン間のスペース

	ImGui::BeginGroup();

	// ユニークなIDを作成
	ImGuiID buttonID = ImGui::GetID(name.c_str());

	// ボタンを生成
	ImGui::PushID(buttonID);
	bool buttonPressed = ImGui::ImageButton((void*)my_shaderData, buttonSize);
	ImGui::PopID();

	if (buttonPressed)
	{
		// ボタンが押されたときの処理
		nowSelectFileName_ = name.c_str();
	}

	// テキストの幅を計算
	std::string fileName = name;
	ImVec2 textSize = ImGui::CalcTextSize(fileName.c_str());

	// ボタンの幅と文字列の長さを比較し、ボタンの幅を超える場合は文字列を切り詰めて "..." を追加
	if (textSize.x > buttonSize.x)
	{
		while (textSize.x > buttonSize.x && fileName.length() > 0)
		{
			fileName.pop_back();
			// テキストのサイズを毎回更新する
			textSize = ImGui::CalcTextSize(fileName.c_str());
		}
		fileName += "...";
		// 合成後のサイズを取得
		textSize = ImGui::CalcTextSize(fileName.c_str());
	}

	float textWidth = textSize.x;
	// テキストの中央揃えを行うためにカーソルの位置を調整
	float posX = (buttonSize.x - textWidth) * 0.5f;
	ImGui::SetCursorPosX(posX + buttonPos.x);
	//ImGui::Text(fileName.c_str());
	if (!CharacterSearch(fileName, "cpp", { 0,1,1,1 }, name)&&
		!CharacterSearch(fileName, "h", { 1,0,1,1 }, name))
	{
		ImGui::Text(fileName.c_str());
	}
	ImGui::EndGroup();

	// ボタンの右端がウィンドウの幅を超える場合は改行する
	if (buttonPos.x + buttonSize.x + buttonSpacing <= ImGui::GetWindowWidth() - 100)
	{
		ImGui::SameLine();
	}
}

void Fream_FileDialog::CreateImage(std::string filePath)
{
	
	// 画像の読み込み
	auto defaultImageSize_ = Vector2();
	//bool ret = LoadBackGroundTextureFromFile(&my_shaderData, &defaultImageSize_.x_, &defaultImageSize_.y_);
	bool ret = false; 
	
	if (my_shaderData_vector_.size() != NULL)
	{
		ret = LoadTextureFromFile(
			filePath.c_str(),// 画像ファイルの場所
			&my_shaderData_vector_.at(my_shaderData_vector_.size() - 1),	// シェーダのデータ作成用の変数
			&defaultImageSize_.x_,// 画像のサイズ取得変数
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
	
}

Fream_FileDialog::FileData::FileData(FileData* parent, std::string name)
{
	parentFile_ = parent;
	myName = name;
}

bool Fream_FileDialog::isHeaderFile(const std::string& filename, const std::string& ext)
{	
	// 拡張子の位置を探す
	std::size_t dotPos = filename.find_last_of(".");
	if (dotPos == std::string::npos) {
		// ドットが見つからない場合は拡張子がないと判断
		return false;
	}

	// 拡張子を取得して比較する
	std::string extension = filename.substr(dotPos + 1);
	if (extension == ext) {
		return true;
	}

	return false;
}

bool Fream_FileDialog::isMatch(const std::string& filepath, const std::string& target) {
	std::filesystem::path pathObj(filepath);
	std::string filename = pathObj.filename().string();

	// 大文字と小文字を区別せずに比較する場合
	/*if (filename == target) {
		return true;
	}*/

	// 大文字と小文字を区別して比較する場合
	// if (filename.compare(target) == 0) {
	//     return true;
	// }

	// ファイル名に指定した文字列が含まれているかを判定する
	if (filename.find(target) != std::string::npos) {
		return true;
	}

	return false;
}

std::wstring Fream_FileDialog::stringToWideString(const std::string& str)
{
	std::wstring wideStr;
	wideStr.resize(str.size());

	std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(
		str.data(), str.data() + str.size(), wideStr.data()
	);

	return wideStr;
}