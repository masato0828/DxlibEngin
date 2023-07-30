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

	icon_ = std::make_unique<CreateIcons>();
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
	fileFullPaht_.clear();

	// DockSpaceの生成処理
	DokingWindow();

	// ファイル名表示ウィンドウの生成処理
	FileNameWindow();

	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_class;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);

	if (ImGui::Begin("ITEM"))
	{
		if (nowSelect_ != nullptr)
		{
			IterateFilesAndFolders();
		}
		HandleContextMenu();
		HandleFileRenamingWindow();

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
		// 強制ドッキング設定
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
	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_class;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar| ImGuiDockNodeFlags_NoResizeFlagsMask_;
	ImGui::SetNextWindowClass(&window_class);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	if (ImGui::Begin("FileNameLog"))
	{
		std::wstring fileName = nowSelectFile_;
		std::wstring extractedText = L"";
		// 文字列を逆順に探索して最後の'\\'の位置を見つける
		size_t lastBackslashPos = fileName.rfind('\\');

		if (lastBackslashPos != std::string::npos) {
			// 最後の'\\'の次の位置から文字列の最後までを抽出
			extractedText = fileName.substr(lastBackslashPos + 1);
			ImGui::Text(Utility::WStringToUTF8(extractedText).c_str());
		}
		else {

			ImGui::Text(Utility::WStringToUTF8(fileName).c_str());
		}

		
		ImGui::End();
	}
	ImGui::PopStyleColor();
}

void Fream_FileDialog_Item::FileLogWindow()
{
	// Imgui用ウィンドウクラスの作成
	ImGuiWindowClass window_class;
	// ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
	window_class.DockNodeFlagsOverrideSet = 
		ImGuiDockNodeFlags_NoWindowMenuButton |
		ImGuiDockNodeFlags_NoTabBar | 
		ImGuiDockNodeFlags_NoResizeFlagsMask_
		;
	ImGui::SetNextWindowClass(&window_class);
	ImGui::Begin("FileLog",nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

	ImGui::Separator();
	
	Recovery(nowSelect_);

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
	// ボタンのサイズと文字の配置を設定する
	if (ImGui::SmallButton(selectData->myName.c_str()))
	{
		std::wstring find = L"\\";
		std::wstring findName = find + Utility::MultiByteToUnicode(nowSelect_->myName);

		size_t found = nowSelectFileName_.find(findName);
		if (found != std::wstring::npos) {
			nowSelectFileName_.erase(found, findName.length());
		}
		nowSelect_ = selectData;
		
		return;
	}

}

void Fream_FileDialog_Item::CreateFiles(
	std::filesystem::path name)
{
	ImVec2 buttonPos = ImGui::GetCursorPos();
	ImVec2 buttonSize = ImVec2(80, 80);
	float buttonSpacing = 10.0f; // ボタン間のスペース

	// グループ作成開始
	ImGui::BeginGroup();

	icon_->MakeFileImage(name,fileFullPaht_);

	// ボタンが左ダブルクリックされ、かつアイテムの上にカーソルがある場合
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
	{
		nowSelectFileName_ = name.c_str();
	}
	// ボタンが左クリックされ、かつアイテムの上にカーソルがある場合
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
	{
		// 現在選択されているファイルが変更された場合
		if (nowSelectFile_ != name.c_str())
		{
			button_click_ = true;
		}
		nowSelectFile_ = name.c_str();

	}
	// ボタンが右クリックされ、かつアイテムの上にカーソルがある場合
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
	{
		// 右クリックされたファイル名を保持し、コンテキストメニューのフラグを立てる
		fileName_ = name.c_str();
		contextMenuFlg_ = true;
	}

	// 左クリックまたは右クリックされた場合
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		// 他のアイテムやウィンドウがホバーされておらず、カーソルがどこにもない場合、コンテキストメニューのフラグを解除する
		if (!ImGui::IsAnyItemHovered() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			contextMenuFlg_ = false;
		}
	}

	ImGui::PopID();



	// ボタンのテキスト部分の処理を行う
	std::wstring fileName = name.filename();
	ImVec2 textSize = ImGui::CalcTextSize(Utility::WStringToUTF8(fileName).c_str());

	// テキストがボタンの幅を超える場合は省略して表示する
	if (textSize.x > buttonSize.x)
	{
		while (textSize.x > buttonSize.x && fileName.length() > 0)
		{
			fileName.pop_back();
			// テキストのサイズを毎回更新する
			textSize = ImGui::CalcTextSize(Utility::WStringToUTF8(fileName).c_str());
		}
		fileName += L"...";
		// 合成後のサイズを取得
		textSize = ImGui::CalcTextSize(Utility::WStringToUTF8(fileName).c_str());
	}

	float textWidth = textSize.x;
	// テキストの中央揃えを行うためにカーソルの位置を調整
	float posX = (buttonSize.x - textWidth) * 0.5f;
	ImGui::SetCursorPosX(posX + buttonPos.x);
	//ImGui::Text(fileName.c_str());

	// 拡張子が"cpp"または"h"でない場合、テキストを表示する(色も変更する)
	if (!Utility::CharacterSearch(Utility::WStringToUTF8(fileName).c_str(), "cpp", { 0,1,1,1 }, Utility::WideStringToString(name)) &&
		!Utility::CharacterSearch(Utility::WStringToUTF8(fileName).c_str(), "h", { 1,0,1,1 }, Utility::WideStringToString(name)))
	{
		ImGui::Text(Utility::WStringToUTF8(fileName).c_str());
	}

	ImGui::EndGroup();


	// ボタンの右端がウィンドウの幅を超える場合は改行する
	if (buttonPos.x + buttonSize.x + buttonSpacing <= ImGui::GetWindowWidth() - 100)
	{
		ImGui::SameLine();
	}
}

void Fream_FileDialog_Item::RenameWindow()
{
	auto path = fileFullPaht_ /= fileName_;

	static char buffer[256] = "";

	// ImGuiのウィンドウを作成
	ImGui::Begin(u8"文字入力");

	ImGui::Text(u8"新しい名前の入力");
	// ImGuiのInputTextを使用してテキスト入力を処理
	ImGui::InputText("##テキスト入力", buffer, sizeof(buffer));
	ImGui::SameLine();
	ImGuiCustom::HelpMarker(
	u8"すいません日本語入力対応してないです、なるべく実装できるようにします"
	);

	if (ImGui::Button(u8"変更の確定"))
	{
		// 入力文字列が変更された場合の処理を行う
		std::string newFileName = buffer;

		// ファイル名の変更処理を実行する
		bool success = Utility::RenameFile(path.u8string(), newFileName);
		if (success)
		{
			// ファイル名の変更が成功した場合の処理を行う
			MessageBox(NULL,"ファイル名の変更に成功しました。","ファイル名の変更",MB_OK);
			context_renameFlg_ = false;
		}
		else
		{
			// ファイル名の変更が失敗した場合の処理を行う
			MessageBox(NULL, "ファイル名の変更に失敗しました。", "ファイル名の変更", MB_OK);
			context_renameFlg_ = false;
		}
	}
	
	// ImGuiのウィンドウの終了
	ImGui::End();
}

void Fream_FileDialog_Item::IterateFilesAndFolders()
{
	// ファイル履歴表示ウィンドウを生成する処理
	FileLogWindow();
	// fileFullPaht_が空であれば現在の作業ディレクトリ(std::filesystem::current_path())を代入
	// 空でなければ現在の作業ディレクトリの後にfileFullPaht_を追加して新しいパスを作成
	fileFullPaht_ = fileFullPaht_.empty() ? std::filesystem::current_path() :
		std::filesystem::current_path() /= fileFullPaht_;


	auto current_path = std::filesystem::current_path();

	// イテレータの作成
	std::filesystem::directory_iterator itr(fileFullPaht_);
	std::filesystem::directory_iterator end{};
	std::wstring directoryName;

	auto copySelectPath = nowSelectFileName_;

	for (; itr != end; ++itr)
	{
		directoryName = itr->path().filename();

		CreateFiles(itr->path()/*現在フォルダの下の階層のファイル及びフォルダの名前*/);


		if (nowSelectFileName_ != copySelectPath)
		{
			fileFullPaht_ /= nowSelectFileName_;
			copySelectPath = nowSelectFileName_;

			// フォルダの場合
			if (std::filesystem::is_directory(fileFullPaht_))
			{
				directoryName = fileFullPaht_.filename();

				nowSelect_->fileMap_.try_emplace(Utility::WideStringToString(directoryName), FileData(nowSelect_, Utility::WideStringToString(directoryName)));
				nowSelect_ = &nowSelect_->fileMap_.at(Utility::WideStringToString(directoryName));
			}
			// ファイルの場合
			else
			{
				Utility::OpenWithDefaultApplication(fileFullPaht_.wstring());
			}
		}
	}
}

void Fream_FileDialog_Item::HandleContextMenu()
{
	if (contextMenuFlg_)
	{
		// ボタン上で右クリックされたときの処理
		ImGui::OpenPopup("ContextMenu");

		Popup();
	}
}

void Fream_FileDialog_Item::HandleFileRenamingWindow()
{
	if (context_renameFlg_)
	{
		RenameWindow();
	}
}


bool& Fream_FileDialog_Item::GetButton_Click()
{
	return button_click_;
}

void Fream_FileDialog_Item::Popup()
{
	if (ImGui::BeginPopup("ContextMenu"))
	{
		// コンテキストメニューの内容をここに記述

		if (ImGui::MenuItem(u8"切り取り")) // ユニークなIDを指定する
		{
			// オプション1が選択されたときの処理
			contextMenuFlg_ = false;
		}

		if (ImGui::MenuItem(u8"コピー")) // ユニークなIDを指定する
		{
			// オプション2が選択されたときの処理
			contextMenuFlg_ = false;
		}

		if (ImGui::MenuItem(u8"貼り付け")) // ユニークなIDを指定する
		{
			// オプション2が選択されたときの処理
			contextMenuFlg_ = false;
		}

		if (ImGui::MenuItem(u8"名前の変更")) // ユニークなIDを指定する
		{
			context_renameFlg_ = true;
			// オプション2が選択されたときの処理
			contextMenuFlg_ = false;
		}
		ImGui::EndPopup();
	}
}
